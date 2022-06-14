// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthItem.h"
#include "HordeCharacter.h"
#include "TimerManager.h"

// On pickup
void AHealthItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHordeCharacter* OtherCharacter = Cast<AHordeCharacter>(OtherActor);
	if (OtherCharacter != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(OtherCharacter->GetController());
		if (PlayerController != nullptr)
		{
			if (OtherCharacter->CollectHealth(HealthAmount))
			{
				Destroy();
			}
			else
			{
				GetWorldTimerManager().SetTimer(DestructionTimer, this, &AHealthItem::DestroyItemActor, DestructionDelay);
			}
		}
	}
}
