// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "WeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API AWeaponPickup : public APickup
{
	GENERATED_BODY()
	
protected:
	void OnGet(AActor* OtherActor) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class TSubclassOf<class AWeapon> WeaponClass;
};
