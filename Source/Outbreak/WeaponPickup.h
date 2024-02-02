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

public:
	AWeaponPickup();
	
protected:
	void OnGet(ARunCharacter* runCharacter) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMesh2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class TSubclassOf<class AWeapon> WeaponClass;
};
