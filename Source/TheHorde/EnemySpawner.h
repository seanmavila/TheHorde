// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AHordeCharacter;

UCLASS()
class THEHORDE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnAI(FVector SpawnLocation, FRotator SpawnRotation, int32 CharacterIndex);

private:

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AHordeCharacter>> CharacterClasses;

};
