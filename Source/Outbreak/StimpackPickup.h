// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "StimpackPickup.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API AStimpackPickup : public APickup
{
	GENERATED_BODY()

protected:
	void OnGet(ARunCharacter* runCharacter) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StimpackSpeed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StimpackDuration = 3;
};
