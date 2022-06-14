// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class THEHORDE_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void Shoot();
	void ReloadAmmo(int32 AmmoToAdd);
	int32 GetClipSize() const;
	int32 CheckClip() const;
	float GetShotDelay() const;
	int32 GetMaxClipSize() const;
	bool IsEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;

	void CanShootAgain();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere)
	USoundBase* EmptySound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.f;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmo;

	UPROPERTY(VisibleAnywhere)
	int32 Ammo;

	UPROPERTY(EditAnywhere)
	float RemoveTextDelay = 1.f;

	UPROPERTY(EditAnywhere)
	float ShotDelay = 1.f;

	bool bHasShot = false;

	FTimerHandle ReloadPromptTimer;

	FTimerHandle ShootingTimer;

};
