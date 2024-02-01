// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "RunCharacter.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(Scene);
	StaticMesh->SetCollisionProfileName("OverlapOnlyPawn");
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);
}

void APickup::OnGet(ARunCharacter* runCharacter)
{
	Destroy();
}

void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* runCharacter = Cast<ARunCharacter>(OtherActor);

	if (runCharacter)
		OnGet(runCharacter);
}

