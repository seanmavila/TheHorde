// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_SetMinAmmo.generated.h"

/**
 * 
 */
UCLASS()
class THEHORDE_API UBTService_SetMinAmmo : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_SetMinAmmo();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
