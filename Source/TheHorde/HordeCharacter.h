// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AmmoItem.h"
#include "HordeCharacter.generated.h"

class AGun;
class USpringArmComponent;

UCLASS()
class THEHORDE_API AHordeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHordeCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Mouse sensitivity 
	virtual void AddControllerPitchInput(float Val) override;

	virtual void AddControllerYawInput(float Val) override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void PullTrigger();

	void ReloadWeapon();

	bool CollectAmmo(int32 AmmoAmount, EAmmoType AmmoType);

	bool CollectHealth(int32 HealthAmount);

	// Blueprint handling
	UFUNCTION(BlueprintPure)
	int32 CheckClip();

	UFUNCTION(BlueprintPure)
	int32 CheckTotalAmmo();

	UFUNCTION(BlueprintPure)
	int32 GetCurrentWeaponIndex();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	bool IsReloading() const;

	UFUNCTION(BlueprintPure)
	bool IsSwapping() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	bool IsAiming() const;

	UFUNCTION(BlueprintPure)
	bool IsEnemy() const;

	UFUNCTION(BlueprintPure)
	bool IsShooting() const;

	UFUNCTION(BlueprintCallable)
	void SetMouseSens(float MouseSens);


private:

	// Movement
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);

	// Dealing and receiving damage
	void StopShooting();
	void StartShooting();
	void DestroyCharacterActor();

	// Weapon swapping
	void WeaponSwap();
	void SwapToLightWeapon();
	void SwapToHeavyWeapon();
	void EquipWeapon(int32 Index);

	// Aiming
	void AimDownSights();
	void HipFireSights();
	USpringArmComponent* GetCameraBoom();

	//Item interaction and drops
	void ItemDrop();
	bool ShouldDropItem();

	// Reloading
	void StopReloading();
	void StopSwapping();

	// UI handling
	void ToggleMenu();
	void NoAmmoPrompt();	
	

	UPROPERTY(EditAnywhere)
	float RotationRate;

	UPROPERTY(EditAnywhere)
	float ControllerRotationRate;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AGun>> GunClasses;

	UPROPERTY()
	TArray<AGun*> Guns;

	UPROPERTY()
	AGun* Gun;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	int32 LightAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 HeavyAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxLightAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxHeavyAmmo;

	UPROPERTY(EditDefaultsOnly)
	float CameraBoomDistADS;

	UPROPERTY(EditDefaultsOnly)
	float CameraBoomDistHipFire;

	UPROPERTY(EditAnywhere)
	float ReloadDelay = 0.95f;

	UPROPERTY(EditAnywhere)
	float SwapDelay = 0.95f;

	UPROPERTY(EditAnywhere)
	float NoAmmoDelay = 1.f;

	UPROPERTY(EditAnywhere)
	float DestructionDelay = 60.f;

	UPROPERTY(EditAnywhere)
	USoundBase* LightSwapSound;

	UPROPERTY(EditAnywhere)
	USoundBase* HeavySwapSound;

	UPROPERTY(EditAnywhere)
	USoundBase* LightAmmoCollectSound;

	UPROPERTY(EditAnywhere)
	USoundBase* HeavyAmmoCollectSound;

	UPROPERTY(EditAnywhere)
	USoundBase* HealthCollectSound;

	UPROPERTY(EditAnywhere)
	USoundBase* WeaponRaiseSound;

	UPROPERTY(EditAnywhere)
	USoundBase* WeaponLowerSound;

	UPROPERTY(EditAnywhere)
	TArray<UClass*> ItemToSpawn;

	UPROPERTY(EditDefaultsOnly)
	bool bShowDefaultWeapon;

	int32 ActiveIndex;

	bool bIsADS;

	bool bIsReloading;

	bool bIsSwapping;

	bool bIsShooting;

	FTimerHandle ReloadTimer;

	FTimerHandle SwapTimer;

	FTimerHandle NoAmmoTimer;

	FTimerHandle DestructionTimer;

	FTimerHandle ShootingTimer;

	int32 AmmoToAdd;

	bool bIsEnemy;

};
