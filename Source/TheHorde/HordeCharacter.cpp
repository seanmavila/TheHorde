// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "TheHordeGameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "HordePlayerController.h"
#include "Kismet/Gameplaystatics.h"
#include "Item.h"
#include "TimerManager.h"
#include "HordeAIController.h"

// Sets default values
AHordeCharacter::AHordeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsADS = false;
	bIsReloading = false;
}

// Called when the game starts or when spawned
void AHordeCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	if (!bShowDefaultWeapon)
	{
		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	}	

	ActiveIndex = 0;
	EquipWeapon(0);
	EquipWeapon(1);

	bIsEnemy = false;
}

// Called every frame
void AHordeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bIsEnemy = Gun->IsEnemy();
}

// Called to bind functionality to input
void AHordeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHordeCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHordeCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AHordeCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AHordeCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AHordeCharacter::StartShooting);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, this, &AHordeCharacter::StopShooting);
	PlayerInputComponent->BindAction(TEXT("WeaponSwap"), EInputEvent::IE_Pressed, this, &AHordeCharacter::WeaponSwap);
	PlayerInputComponent->BindAction(TEXT("SwapToLight"), EInputEvent::IE_Pressed, this, &AHordeCharacter::SwapToLightWeapon);
	PlayerInputComponent->BindAction(TEXT("SwapToHeavy"), EInputEvent::IE_Pressed, this, &AHordeCharacter::SwapToHeavyWeapon);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AHordeCharacter::ReloadWeapon);
	PlayerInputComponent->BindAction(TEXT("ADS"), EInputEvent::IE_Pressed, this, &AHordeCharacter::AimDownSights);
	PlayerInputComponent->BindAction(TEXT("ADS"), EInputEvent::IE_Released, this, &AHordeCharacter::HipFireSights);
	PlayerInputComponent->BindAction(TEXT("ToggleMenu"), EInputEvent::IE_Pressed, this, &AHordeCharacter::ToggleMenu);
}

// Movement
void AHordeCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AHordeCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AHordeCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * ControllerRotationRate * GetWorld()->GetDeltaSeconds());
}

void AHordeCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * ControllerRotationRate * GetWorld()->GetDeltaSeconds());
}

void AHordeCharacter::AddControllerPitchInput(float Val) // override for mouse sensitivity 
{
	Super::AddControllerPitchInput(Val * RotationRate);
}

void AHordeCharacter::AddControllerYawInput(float Val) // override for mouse sensitivity 
{
	Super::AddControllerYawInput(Val * RotationRate);
}

// Dealing and receiving damage
float AHordeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	if (IsDead())
	{
		ATheHordeGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ATheHordeGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		ItemDrop();
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetWorldTimerManager().SetTimer(DestructionTimer, this, &AHordeCharacter::DestroyCharacterActor, DestructionDelay);
	}

	return DamageToApply;
}

void AHordeCharacter::StartShooting()
{
	PullTrigger();
	if (ActiveIndex == 0)
	{
		GetWorldTimerManager().SetTimer(ShootingTimer, this, &AHordeCharacter::PullTrigger, Gun->GetShotDelay(), true);
	}
}

void AHordeCharacter::PullTrigger() // calls gun shoot method
{
	if (!GetWorldTimerManager().IsTimerActive(ReloadTimer) && !GetWorldTimerManager().IsTimerActive(SwapTimer))
	{
		bIsShooting = true;
		Gun->Shoot();
	}
}

void AHordeCharacter::StopShooting()
{
	bIsShooting = false;
	GetWorldTimerManager().ClearTimer(ShootingTimer);
}

void AHordeCharacter::DestroyCharacterActor()
{
	if (this)
	{
		Destroy();
	}
}

bool AHordeCharacter::IsEnemy() const
{
	return bIsEnemy;
}

bool AHordeCharacter::IsDead() const
{
	return Health <= 0;
}

float AHordeCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

bool AHordeCharacter::IsShooting() const
{
	if (ActiveIndex == 0)
	{
		return bIsShooting;
	}
	else
	{
		if (GetWorldTimerManager().IsTimerActive(ShootingTimer))
		{
			return true;
		}
	}
	return false;
}

void AHordeCharacter::SetMouseSens(float MouseSens)
{
	RotationRate = MouseSens;
}



// Weapon swapping
void AHordeCharacter::WeaponSwap() // Swaping using mouse wheel
{
	if (!GetWorldTimerManager().IsTimerActive(SwapTimer))
	{
		if (Gun)
		{
			bIsSwapping = true;
			GetWorldTimerManager().SetTimer(SwapTimer, this, &AHordeCharacter::StopSwapping, SwapDelay);
			if (Guns.Num() > ActiveIndex + 1)
			{
				++ActiveIndex;
				UGameplayStatics::SpawnSoundAttached(HeavySwapSound, GetMesh(), TEXT("WeaponSocket"));
			}
			else
			{
				ActiveIndex = 0;
				UGameplayStatics::SpawnSoundAttached(LightSwapSound, GetMesh(), TEXT("WeaponSocket"));

			}
			if (AGun* NextGun = Guns[ActiveIndex])
			{
				Gun->SetActorHiddenInGame(true);
				Gun = NextGun;
				Gun->SetActorHiddenInGame(false);
			}
		}
	}
	
}

void AHordeCharacter::SwapToLightWeapon() // Swaping using keyboard
{

	if (ActiveIndex != 0)
	{
		if (!GetWorldTimerManager().IsTimerActive(SwapTimer))
		{
			if (Gun)
			{
				bIsSwapping = true;
				GetWorldTimerManager().SetTimer(SwapTimer, this, &AHordeCharacter::StopSwapping, SwapDelay);
				ActiveIndex = 0;
				if (AGun* NextGun = Guns[ActiveIndex])
				{
					Gun->SetActorHiddenInGame(true);
					Gun = NextGun;
					Gun->SetActorHiddenInGame(false);
					UGameplayStatics::SpawnSoundAttached(LightSwapSound, GetMesh(), TEXT("WeaponSocket"));
				}
			}
		}
	}
}

void AHordeCharacter::SwapToHeavyWeapon() // Swaping using keyboard
{
	if (ActiveIndex != 1)
	{
		if (!GetWorldTimerManager().IsTimerActive(SwapTimer))
		{
			if (Gun)
			{
				bIsSwapping = true;
				GetWorldTimerManager().SetTimer(SwapTimer, this, &AHordeCharacter::StopSwapping, SwapDelay);
				ActiveIndex = 1;
				if (AGun* NextGun = Guns[ActiveIndex])
				{
					Gun->SetActorHiddenInGame(true);
					Gun = NextGun;
					Gun->SetActorHiddenInGame(false);
					UGameplayStatics::SpawnSoundAttached(HeavySwapSound, GetMesh(), TEXT("WeaponSocket"));
				}
			}
		}
	}
}

void AHordeCharacter::EquipWeapon(int32 Index) // Adds weapon to gun array
{
	if (AGun* NewGun = GetWorld()->SpawnActor<AGun>(GunClasses[Index]))
	{
		if (Index > 0)
		{
			NewGun->SetHidden(true);
		}
		else
		{
			Gun = NewGun;
		}
		NewGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		NewGun->SetOwner(this);
		Guns.Add(NewGun);
	}
}

bool AHordeCharacter::IsSwapping() const
{
	return bIsSwapping;
}

void AHordeCharacter::StopSwapping()
{
	bIsSwapping = false;
}

int32 AHordeCharacter::GetCurrentWeaponIndex()
{
	return ActiveIndex;
}

// Aiming
void AHordeCharacter::AimDownSights()
{
	
	if (USpringArmComponent* thirdPersonCamera = GetCameraBoom())
	{
		thirdPersonCamera->TargetArmLength = CameraBoomDistADS;
		UGameplayStatics::SpawnSoundAttached(WeaponRaiseSound, GetMesh(), TEXT("WeaponSocket"));
		
		bIsADS = true;
	}
	
}

void AHordeCharacter::HipFireSights()
{
	
	if (USpringArmComponent* thirdPersonCamera = GetCameraBoom())
	{
		thirdPersonCamera->TargetArmLength = CameraBoomDistHipFire;
		UGameplayStatics::SpawnSoundAttached(WeaponLowerSound, GetMesh(), TEXT("WeaponSocket"));

		bIsADS = false;
	}
}

USpringArmComponent* AHordeCharacter::GetCameraBoom()
{
	USpringArmComponent* CameraBoom = Cast<USpringArmComponent>(GetComponentByClass(USpringArmComponent::StaticClass()));
	if (CameraBoom)
	{
		return CameraBoom;
	}
	return nullptr;
}

bool AHordeCharacter::IsAiming() const
{
	return bIsADS;
}


// Item drops and interactions
void AHordeCharacter::ItemDrop()
{
	if (ShouldDropItem())
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (!PlayerController)
		{

			FVector SpawnLocation = GetActorLocation();
			FRotator SpawnRotation = GetActorRotation();
			int32 SpawnIndex;
			int32 SpawnWeight = FMath::RandRange(0, 100);
			if (SpawnWeight <= 50)
			{
				SpawnIndex = 0;
			}
			else if (SpawnWeight > 50 && SpawnWeight <= 75)
			{
				SpawnIndex = 1;
			}
			else
			{
				SpawnIndex = 2;
			}
			GetWorld()->SpawnActor(ItemToSpawn[SpawnIndex], &SpawnLocation, &SpawnRotation);
		}
	}
	
}

bool AHordeCharacter::CollectHealth(int32 HealthAmount)
{
	if (Health < MaxHealth)
	{
		Health = FMath::Min(Health + HealthAmount, MaxHealth);
		UGameplayStatics::SpawnSoundAttached(HealthCollectSound, GetMesh(), TEXT("WeaponSocket"));
		return true;
	}
	return false;

}

bool AHordeCharacter::CollectAmmo(int32 AmmoAmount, EAmmoType AmmoType)
{

	switch (AmmoType)
	{
	case EAmmoType::E_Light:
		if (LightAmmo < MaxLightAmmo)
		{
			LightAmmo = FMath::Min(LightAmmo + AmmoAmount, MaxLightAmmo);
			UGameplayStatics::SpawnSoundAttached(LightAmmoCollectSound, GetMesh(), TEXT("WeaponSocket"));
			return true;
		}
		break;
	case EAmmoType::E_Heavy:
		if (HeavyAmmo < MaxHeavyAmmo)
		{
			HeavyAmmo = FMath::Min(HeavyAmmo + AmmoAmount, MaxHeavyAmmo);
			UGameplayStatics::SpawnSoundAttached(HeavyAmmoCollectSound, GetMesh(), TEXT("WeaponSocket"));
			return true;
		}
		break;
	default:
		return false;
		break;
	}
	return false;
}

bool AHordeCharacter::ShouldDropItem()
{
	int32 DropRate = FMath::RandRange(0, 100);
	if (DropRate >= 40)
	{
		return true;
	}
	return false;
}

// Reloading
void AHordeCharacter::ReloadWeapon()
{

	if (Gun->GetClipSize() > 0)
	{
		if (!bIsReloading)
		{
			bIsReloading = true;
			GetWorldTimerManager().SetTimer(ReloadTimer, this, &AHordeCharacter::StopReloading, ReloadDelay);
			switch (ActiveIndex)
			{
			case 0:
				AmmoToAdd = FMath::Min(LightAmmo, Gun->GetClipSize());
				if (LightAmmo >= AmmoToAdd && LightAmmo > 0)
				{
					LightAmmo -= AmmoToAdd;
					Gun->ReloadAmmo(AmmoToAdd);
				}
				else
				{
					NoAmmoPrompt();
				}
				break;

			case 1:
				AmmoToAdd = FMath::Min(HeavyAmmo, Gun->GetClipSize());
				if (HeavyAmmo >= AmmoToAdd && HeavyAmmo > 0)
				{
					HeavyAmmo -= AmmoToAdd;
					Gun->ReloadAmmo(AmmoToAdd);
				}
				else
				{
					NoAmmoPrompt();
				}
				break;
			default:
				break;
			}
		}
	}

}

void AHordeCharacter::StopReloading()
{
	bIsReloading = false;
}

int32 AHordeCharacter::CheckClip()
{
	return Gun->CheckClip();
}

int32 AHordeCharacter::CheckTotalAmmo()
{
	if (ActiveIndex == 0)
	{
		return LightAmmo;
	}
	return HeavyAmmo;
}

bool AHordeCharacter::IsReloading() const
{
	return bIsReloading;
}

// UI Handling

void AHordeCharacter::ToggleMenu()
{
	AHordePlayerController* PlayerController = Cast<AHordePlayerController>(GetController());
	if (PlayerController != nullptr && !IsDead())
	{
		PlayerController->ToggleMenu();
	}
}

void AHordeCharacter::NoAmmoPrompt()
{
	AHordePlayerController* PlayerController = Cast<AHordePlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		if (!GetWorldTimerManager().IsTimerActive(NoAmmoTimer))
		{
			PlayerController->AddNoAmmo();
			GetWorldTimerManager().SetTimer(NoAmmoTimer, PlayerController, &AHordePlayerController::RemoveNoAmmo, NoAmmoDelay);
		}		
	}
}




