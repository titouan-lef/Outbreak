// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle.h"
#include "Mud.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API AMud : public AObstacle
{
	GENERATED_BODY()

public:
	AMud();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ActiveEffect(ACharacter* character) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MudStats")
	float SlowSpeed = 100;

private:
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType);
};
