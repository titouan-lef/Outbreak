// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Tile.generated.h"

class UKismetMathLibrary;
class UComponents;

// Tile is a name which is shown on the Blueprint node
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FExitedSignature, class ATile*, Tile);

UCLASS()
class OUTBREAK_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION()
	void Remove();

	UFUNCTION()
	FTransform GetTransformAttachPoint();

	// EVENT DISPATCHERS
	UPROPERTY(BlueprintAssignable)
	FExitedSignature OnExited;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StaticMeshFloor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StaticMeshWallLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StaticMeshWallRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* ExitTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* SpawnZone;

	// ZOMBIES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	TArray<TSubclassOf<class AZombie>> ZombieClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	int MaxZombies = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie")
	float ZombieSpawnChance = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zombie")
	class UBoxComponent* DestroyZone;

	// OBSTACLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	TArray<TSubclassOf<class AObstacle>> ObstacleClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	int MaxObstacles = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	float ObstacleSpawnChance = 0.5f;


	// RUN CHARACTER
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class TSubclassOf<class ARunCharacter> RunCharacterCLass;


	// FUNCTIONS
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void SpawnManyZombie();

	bool CheckOverlapAtLocation(FVector spawnLocation);

	template <typename T>
	void SpawnManyActor(TArray<TSubclassOf<T>> actorClasses, int maxActor, float spawnChance)
	{
		if (actorClasses.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("No actorClasses"));
			return;
		}

		for (int i = 0; i < maxActor; i++)
		{
			float random = UKismetMathLibrary::RandomFloatInRange(0, 1);
			if (random < spawnChance)
				SpawnActor(actorClasses);
		}
	}

	template <typename T>
	void SpawnActor(TArray<TSubclassOf<T>> actorClasses)
	{
		int randomIndex = UKismetMathLibrary::RandomIntegerInRange(0, actorClasses.Num() - 1);
		if (!actorClasses[randomIndex])
		{
			UE_LOG(LogTemp, Warning, TEXT("actorClasses[%d] is nullptr"), randomIndex);
			return;
		}

		bool overlap = true;
		int maxAttempts = 10;  // Limit the number of attempts to spawn an actor

		FVector spawnLocation;
		for (int attempt = 0; attempt < maxAttempts && overlap; ++attempt)
		{
			spawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnZone->Bounds.Origin, SpawnZone->Bounds.BoxExtent);

			// Check if there is an overlap at the location
			overlap = CheckOverlapAtLocation(spawnLocation);
		}

		if (overlap)
		{
			UE_LOG(LogTemp, Warning, TEXT("No location found"));
			return;
		}

		//float randomYaw = UKismetMathLibrary::RandomFloatInRange(0, 360);
		FRotator spawnRotation = FRotator(0, 0, 0);

		// Create and attach a child component to parent actor
		UChildActorComponent* childActorComponent = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass());
		childActorComponent->RegisterComponent();
		childActorComponent->SetChildActorClass(actorClasses[randomIndex]);

		// Add the child component your parent actor
		AddOwnedComponent(childActorComponent);

		// Define the position & rotation of the child component
		childActorComponent->SetRelativeLocationAndRotation(spawnLocation, spawnRotation);
	}
};