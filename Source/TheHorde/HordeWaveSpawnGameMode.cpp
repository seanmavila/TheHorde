// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeWaveSpawnGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "HordeAIController.h"
#include "HordePlayerController.h"
#include "EnemySpawner.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AHordeWaveSpawnGameMode::AHordeWaveSpawnGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsSpawning = false;
	CurrentWave = 0;
	EnemyKnt = TotalEnemyKnt = EnemiesRemaining = 0;
}

void AHordeWaveSpawnGameMode::BeginPlay()
{
	Super::BeginPlay();
	WaveStart();
}

void AHordeWaveSpawnGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SpawnManager();
}

//Spawn Management

void AHordeWaveSpawnGameMode::SpawnEnemyAI()
{
	if (EnemySpawner != nullptr)
	{
		AEnemySpawner* Spawner = GetWorld()->SpawnActor<AEnemySpawner>(EnemySpawner);
		if (Spawner != nullptr)
		{
			int32 SpawnLoc = FMath::RandRange(0, 2);
			int32 CharacterIndex = FMath::RandRange(0, 2);
			int32 val = *CharacterMap.Find(CharacterIndex);
			if (val > 0)
			{
				switch (SpawnLoc)
				{
				case 0:
					Spawner->SpawnAI(SpawnOneLocation, SpawnOneRotation, CharacterIndex);
					break;
				case 1:
					Spawner->SpawnAI(SpawnTwoLocation, SpawnTwoRotation, CharacterIndex);
					break;
				case 2:
					Spawner->SpawnAI(SpawnThreeLocation, SpawnThreeRotation, CharacterIndex);
					break;
				default:
					break;
				}
				CharacterMap.Add(CharacterIndex, val - 1);
			}
			else
			{
				SpawnEnemyAI();
			}
		}
	}
}

void AHordeWaveSpawnGameMode::SpawnManager()
{
	if (EnemyKnt < TotalEnemyKnt)
	{
		if (!bIsSpawning)
		{
			bIsSpawning = true;
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &AHordeWaveSpawnGameMode::StopSpawn, SpawnDelay);
			SpawnEnemyAI();
			EnemyKnt++;
		}
	}

}

void AHordeWaveSpawnGameMode::StopSpawn()
{
	bIsSpawning = false;
}

//Wave Management

// What to do when a pawn is killed, i.e. decrement wave count and/or end the game
void AHordeWaveSpawnGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);



	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		EndGame(false);
	}
	
	EnemiesRemaining--;

	

	for (AHordeAIController* Controller : TActorRange<AHordeAIController>(GetWorld()))
	{
		if (!Controller->IsDead() || EnemiesRemaining > 0)
		{
			return;
		}
	}
	EndGame(true);
}

int32 AHordeWaveSpawnGameMode::GetCurrentWave()
{
	return CurrentWave;
}

int32 AHordeWaveSpawnGameMode::GetEnemiesRemaining()
{
	return EnemiesRemaining;
}

void AHordeWaveSpawnGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		if (bIsWinner)
		{
			GetWorldTimerManager().SetTimer(WaveTimer, this, &AHordeWaveSpawnGameMode::WaveStart, WaveDelay);
		}
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}



void AHordeWaveSpawnGameMode::WaveStart()
{
	CurrentWave++;

	WaveAnnouncement();

	LightEnemyKnt += 5;
	
	if (CurrentWave >= 5)
	{
		MediumEnemyKnt += 2;
	}
	if (CurrentWave >= 10)
	{
		HeavyEnemyKnt += 1;
	}

	CharacterMap.Add(0, LightEnemyKnt);
	CharacterMap.Add(1, MediumEnemyKnt);
	CharacterMap.Add(2, HeavyEnemyKnt);

	TotalEnemyKnt = LightEnemyKnt + MediumEnemyKnt + HeavyEnemyKnt;
	EnemiesRemaining = TotalEnemyKnt;
	EnemyKnt = 0;
}

// Controlls wave announcement text and UI
void AHordeWaveSpawnGameMode::WaveAnnouncement()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn != nullptr)
	{
		AHordePlayerController* PlayerController = Cast<AHordePlayerController>(PlayerPawn->GetController());
		if (PlayerController != nullptr)
		{
			PlayerController->AddWaveStartText();
			GetWorldTimerManager().SetTimer(WaveStartTextTimer, PlayerController, &AHordePlayerController::RemoveWaveStartText, WaveStartTextDelay);
		}
	}
}
