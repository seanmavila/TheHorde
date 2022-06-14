// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeAIController.h"
#include "Kismet/Gameplaystatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HordeCharacter.h"

void AHordeAIController::BeginPlay()
{
	Super::BeginPlay();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		//GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
	
}

void AHordeAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

bool AHordeAIController::IsDead() const
{
	AHordeCharacter* ControlledCharacter = Cast<AHordeCharacter>(GetPawn());
	if (ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead();
	}

	return true;
}

