// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoItem.h"
#include "HordeCharacter.h"
#include "TimerManager.h"

// Pickup ammo
void AAmmoItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHordeCharacter* OtherCharacter = Cast<AHordeCharacter>(OtherActor);
	if (OtherCharacter != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(OtherCharacter->GetController());
		if (PlayerController != nullptr)
		{
			if (OtherCharacter->CollectAmmo(AmmoAmount, AmmoType))
			{
				Destroy();
			}
			else
			{
				GetWorldTimerManager().SetTimer(DestructionTimer, this, &AAmmoItem::DestroyItemActor, DestructionDelay);
			}
		}
	}
	
}

