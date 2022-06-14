// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetMinAmmo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_SetMinAmmo::UBTService_SetMinAmmo()
{
	NodeName = TEXT("Set Min Ammo");
}

void UBTService_SetMinAmmo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), 0);
}
