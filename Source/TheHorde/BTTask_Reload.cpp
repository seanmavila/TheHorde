// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Reload.h"
#include "AIController.h"
#include "HordeCharacter.h"

UBTTask_Reload::UBTTask_Reload()
{
	NodeName = TEXT("Reload");
}

EBTNodeResult::Type UBTTask_Reload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AHordeCharacter* Character = Cast<AHordeCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->ReloadWeapon();

	return EBTNodeResult::Succeeded;
}
