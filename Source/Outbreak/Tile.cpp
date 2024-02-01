// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "RunCharacter.h"
#include "Zombie.h"
#include "Obstacle.h"
#include "Pickup.h"

#include "NotStackingActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

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

	SpawnManyZombie();

	SpawnManyActor(ObstacleClasses, MaxObstacles, ObstacleSpawnChance);

	SpawnManyActor(PickupClasses, MaxPickups, PickupSpawnChance);
}

void ATile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* runCharacter = Cast<ARunCharacter>(OtherActor);

	if (runCharacter)
		OnExited.Broadcast(this);
}

void ATile::SpawnManyZombie()
{

	if (ZombieClasses.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ZombieClasses"));
		return;
	}

	for (int i = 0; i < MaxZombies; i++)
	{
		int randomIndex = UKismetMathLibrary::RandomIntegerInRange(0, ZombieClasses.Num() - 1);
		if (!ZombieClasses[randomIndex])
		{
			UE_LOG(LogTemp, Warning, TEXT("ZombieClasses[%d] is nullptr"), randomIndex);
			return;
		}

		float random = UKismetMathLibrary::RandomFloatInRange(0, 1);
		if (random < ZombieSpawnChance)
		{
			//float randomYaw = UKismetMathLibrary::RandomFloatInRange(0, 360);
			FRotator spawnRotation = FRotator(0, 180, 0);

			FVector spawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnZone->GetComponentLocation(), SpawnZone->Bounds.BoxExtent);
			AZombie* zombie = GetWorld()->SpawnActor<AZombie>(ZombieClasses[randomIndex], spawnLocation, spawnRotation);
			SetZombieLocation(zombie, spawnLocation);

			// Verifications
			bool overlap = CheckOverlapAtLocation(zombie);// Check if there is an overlap at the location
			int maxAttempts = 10;  // Limit the number of attempts to spawn an actor

			for (int attempt = 0; attempt < maxAttempts && overlap; ++attempt)
			{
				spawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnZone->Bounds.Origin, SpawnZone->Bounds.BoxExtent);
				SetZombieLocation(zombie, spawnLocation);
				overlap = CheckOverlapAtLocation(zombie);
			}

			if (overlap)
			{
				zombie->Destroy();
				UE_LOG(LogTemp, Warning, TEXT("No location found"));
			}
		}
	}
}

void ATile::SetZombieLocation(AZombie* zombie, FVector spawnLocation)
{
	float zCoord = -zombie->GetMesh()->GetRelativeLocation().Z;
	zombie->SetActorLocation(FVector(spawnLocation.X, spawnLocation.Y, zCoord));
}

bool ATile::CheckOverlapAtLocation(AActor* actor)
{
	TArray<AActor*> OverlappingActors;
	actor->GetOverlappingActors(OverlappingActors);

	for (AActor* actorTmp : OverlappingActors)
	{
		INotStackingActor* notStackingActor = Cast<INotStackingActor>(actorTmp);
		if (notStackingActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlap entre %s et %s"), *actor->GetName(), *actorTmp->GetName());
			return true;
		}
	}

	return false;
}