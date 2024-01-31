// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "RunCharacter.h"
#include "Zombie.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

#include "Kismet/KismetMathLibrary.h"

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

	SpawnZone = CreateDefaultSubobject<UBoxComponent>("SpawnZone");
	SpawnZone->SetupAttachment(Scene);

	DestroyZone = CreateDefaultSubobject<UBoxComponent>("DestroyZone");
	DestroyZone->SetupAttachment(Scene);
}

void ATile::Remove()
{
	TArray<AActor*> OverlappingActors;
	DestroyZone->GetOverlappingActors(OverlappingActors, AZombie::StaticClass());

	for (AActor* actor : OverlappingActors)
		actor->Destroy();

	Destroy();
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

	SpawnManyZombies();
}

void ATile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* runCharacter = Cast<ARunCharacter>(OtherActor);

	if (runCharacter)
		OnExited.Broadcast(this);
}

void ATile::SpawnManyZombies()
{
	for (int i = 0; i < MaxZombies; i++)
	{
		float random = UKismetMathLibrary::RandomFloatInRange(0, 1);
		if (random < SpawnChance)
			SpawnZombie();
	}
}

void ATile::SpawnZombie()
{
	if (ZombieClasses.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ZombieClasses"));
		return;
	}

	FVector spawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnZone->GetComponentLocation(), SpawnZone->Bounds.BoxExtent);
	spawnLocation.Z = 20;

	//float randomYaw = UKismetMathLibrary::RandomFloatInRange(0, 360);
	FRotator spawnRotation = FRotator(1, 1, 1);

	int randomIndex = UKismetMathLibrary::RandomIntegerInRange(0, ZombieClasses.Num() - 1);

	if (!ZombieClasses[randomIndex])
	{
		UE_LOG(LogTemp, Warning, TEXT("ZombieClasses[%d] is nullptr"), randomIndex);
		return;
	}

	GetWorld()->SpawnActor<AZombie>(ZombieClasses[randomIndex], spawnLocation, spawnRotation);
}

