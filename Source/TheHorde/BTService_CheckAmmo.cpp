// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAmmo.h"
#include "HordeCharacter.h"
#include "HordeAIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckAmmo::UBTService_CheckAmmo()
{
	NodeName = TEXT("Check Ammo");
}

void UBTService_CheckAmmo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return;
	}

	AHordeCharacter* Character = Cast<AHordeCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (Character == nullptr)
	{
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), Character->CheckClip());
}
