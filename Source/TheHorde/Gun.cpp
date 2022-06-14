// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/Gameplaystatics.h"
#include "DrawDebugHelpers.h"
#include "HordeAIController.h"
#include "EngineUtils.h"
#include "HordeCharacter.h"
#include "HordeAIController.h"
#include "HordePlayerController.h"
#include "TimerManager.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}


// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	Ammo = MaxAmmo;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return nullptr;
	}
	return OwnerPawn->GetController();
}

// Shooting section
bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
	{
		return false;
	}
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);


	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;

	TArray<FVector> Accuracy;
	Accuracy.Add(End);
	Accuracy.Add(End + FVector(0, FMath::RandRange(-1000, 1000), FMath::RandRange(-1000, 1000)));

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());


	for (AHordeCharacter* GameCharacter : TActorRange<AHordeCharacter>(GetWorld()))
	{
		if (GameCharacter->IsDead())
		{
			Params.AddIgnoredActor(GameCharacter);
		}
	}

	AHordeAIController* EnemyAIController = Cast<AHordeAIController>(OwnerController);

	if (EnemyAIController != nullptr)
	{
		return GetWorld()->LineTraceSingleByChannel(Hit, Location, Accuracy[FMath::RandRange(0, 1)], ECollisionChannel::ECC_GameTraceChannel1, Params);
	}
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

bool AGun::IsEnemy()
{
	FHitResult Hit;
	FVector ShotDirection;
	if (GunTrace(Hit, ShotDirection))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			if (HitActor->ActorHasTag("Enemy"))
			{
				return true;
			}
		}
	}
	return false;
}

void AGun::Shoot()
{
	if (Ammo > 0)
	{
		if (!bHasShot)
		{
			bHasShot = true;
			GetWorldTimerManager().SetTimer(ShootingTimer, this, &AGun::CanShootAgain, ShotDelay);
			Ammo--;
			//UE_LOG(LogTemp, Warning, TEXT("Current ammo: %i"), Ammo);
			UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
			UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

			FHitResult Hit;
			FVector ShotDirection;
			if (GunTrace(Hit, ShotDirection))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

				AActor* HitActor = Hit.GetActor();
				if (HitActor != nullptr)
				{
					FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
					AController* OwnerController = GetOwnerController();
					if (OwnerController == nullptr)
					{
						return;
					}
					HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
				}
			}
		}
	}
	else
	{
		AHordePlayerController* PlayerController = Cast<AHordePlayerController>(GetOwnerController());
		if (PlayerController != nullptr)
		{
			if (!GetWorldTimerManager().IsTimerActive(ReloadPromptTimer))
			{
				PlayerController->AddReloadPrompt();
				GetWorldTimerManager().SetTimer(ReloadPromptTimer, PlayerController, &AHordePlayerController::RemoveReloadPrompt, RemoveTextDelay);
			}
		}
		UGameplayStatics::SpawnSoundAttached(EmptySound, Mesh, TEXT("MuzzleFlashSocket"));
	}

}

void AGun::CanShootAgain()
{
	bHasShot = false;
}

float AGun::GetShotDelay() const
{
	return ShotDelay;
}


// Reload Section
void AGun::ReloadAmmo(int32 AmmoToAdd)
{
	UGameplayStatics::SpawnSoundAttached(ReloadSound, Mesh, TEXT("MuzzleFlashSocket"));
	Ammo += AmmoToAdd;
}

int32 AGun::GetClipSize() const
{
	return MaxAmmo - Ammo;
}

int32 AGun::CheckClip() const
{
	return Ammo;
}

int32 AGun::GetMaxClipSize() const
{
	return MaxAmmo;
}





