// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoItem.generated.h"

UENUM()
enum class EAmmoType : uint8
{
	E_Light UMETA(DisplayName = "Light Ammo"),
	E_Heavy UMETA(DisplayName = "Heavy Ammo")
};

UCLASS()
class THEHORDE_API AAmmoItem : public AItem
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	int32 AmmoAmount;

	UPROPERTY(EditAnywhere)
	EAmmoType AmmoType;

	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
