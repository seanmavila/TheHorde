// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "HordeCharacter.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Spawn a single HordeCharacter
void AEnemySpawner::SpawnAI(FVector SpawnLocation, FRotator SpawnRotation, int32 CharacterIndex)
{
	AHordeCharacter* EnemyCharacter = GetWorld()->SpawnActor<AHordeCharacter>(CharacterClasses[CharacterIndex], SpawnLocation, SpawnRotation);
}

