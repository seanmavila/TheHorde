// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheHordeGameModeBase.h"
#include "HordeWaveSpawnGameMode.generated.h"

/**
 * 
 */
UCLASS()
class THEHORDE_API AHordeWaveSpawnGameMode : public ATheHordeGameModeBase
{
	GENERATED_BODY()

public:
	AHordeWaveSpawnGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void PawnKilled(APawn* PawnKilled) override;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentWave();

	UFUNCTION(BlueprintPure)
	int32 GetEnemiesRemaining();

	void WaveAnnouncement();

private:

	void SpawnEnemyAI();

	void SpawnManager();
	
	void StopSpawn();
	
	void WaveStart();

	void EndGame(bool bIsPlayerWinner);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemySpawner> EnemySpawner;

	UPROPERTY(EditAnywhere)
	FVector SpawnOneLocation;

	UPROPERTY(EditAnywhere)
	FRotator SpawnOneRotation;

	UPROPERTY(EditAnywhere)
	FVector SpawnTwoLocation;

	UPROPERTY(EditAnywhere)
	FRotator SpawnTwoRotation;

	UPROPERTY(EditAnywhere)
	FVector SpawnThreeLocation;

	UPROPERTY(EditAnywhere)
	FRotator SpawnThreeRotation;

	UPROPERTY(EditAnywhere)
	int32 LightEnemyKnt = 10;

	UPROPERTY(EditAnywhere)
	int32 MediumEnemyKnt = 0;

	UPROPERTY(EditAnywhere)
	int32 HeavyEnemyKnt = 0;

	UPROPERTY(EditAnywhere)
	float SpawnDelay;

	UPROPERTY(EditAnywhere)
	float WaveDelay;

	UPROPERTY(EditAnywhere)
	float WaveStartTextDelay;

	bool bIsSpawning;

	int32 CurrentWave;

	int32 EnemyKnt;

	int32 TotalEnemyKnt;

	int32 EnemiesRemaining;

	TMap<int32, int32> CharacterMap;

	FTimerHandle SpawnTimer;

	FTimerHandle WaveTimer;

	FTimerHandle WaveStartTextTimer;
};
