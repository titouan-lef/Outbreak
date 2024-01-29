// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "RunCharacter.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	StaticMeshFloor = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshFloor");
	StaticMeshFloor->SetupAttachment(Scene);

	StaticMeshWallLeft = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshWallLeft");
	StaticMeshWallLeft->SetupAttachment(Scene);

	StaticMeshWallRight = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshWallRight");
	StaticMeshWallRight->SetupAttachment(Scene);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>("AttachPoint");
	AttachPoint->SetupAttachment(Scene);

	ExitTrigger = CreateDefaultSubobject<UBoxComponent>("ExitTrigger");
	ExitTrigger->SetupAttachment(Scene);
}

FTransform ATile::GetTransformAttachPoint()
{
	return AttachPoint->GetComponentTransform();
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
	ExitTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATile::OnBoxBeginOverlap);
}

void ATile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* runCharacter = Cast<ARunCharacter>(OtherActor);

	if (runCharacter)
		OnExited.Broadcast(this);
}

