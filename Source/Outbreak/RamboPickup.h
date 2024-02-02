// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "RamboPickup.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API ARamboPickup : public APickup
{
	GENERATED_BODY()

protected:
	void OnGet(ARunCharacter* runCharacter) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RamboModeDuration = 3;
};
