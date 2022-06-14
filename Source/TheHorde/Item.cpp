// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "HordeCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetCollisionProfileName("Trigger");
	RootComponent = BoxCollider;

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Mesh->SetupAttachment(BoxCollider);

	RotationRate = FRotator(0.f, 180.f, 0.f);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(RotationRate * DeltaTime * RotationSpeed);

}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AItem::DestroyItemActor()
{
	if (this)
	{
		Destroy();
	}
}

