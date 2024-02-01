// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle.h"
#include "Fence.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API AFence : public AObstacle
{
	GENERATED_BODY()

public:
	AFence();

protected:
	void ActiveEffect(ACharacter* character) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fence")
	class UBoxComponent* BlockCollision;
	
};
