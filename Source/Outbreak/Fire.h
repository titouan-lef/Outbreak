// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle.h"
#include "Damageable.h"
#include "Fire.generated.h"

class IDamageable;

/**
 * 
 */
UCLASS()
class OUTBREAK_API AFire : public AObstacle
{
	GENERATED_BODY()

public:
	AFire();
	
protected:
	void ActiveEffect(ACharacter* character) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float FireDamage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	int NbRepetition = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float DamageDelay = 0.5;
};
