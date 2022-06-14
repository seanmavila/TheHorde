// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "HealthItem.generated.h"

/**
 * 
 */
UCLASS()
class THEHORDE_API AHealthItem : public AItem
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		int32 HealthAmount;

	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
