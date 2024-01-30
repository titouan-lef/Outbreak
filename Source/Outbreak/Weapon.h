// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

	UFUNCTION()
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	FString GetName();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	int GetCurrentAmmo();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UArrowComponent* Muzzle;

	// PROPERTIES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon stats")
	FString Name = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon stats")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon stats")
	int InitialAmmo = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon stats")
	int CurrentAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

private:

	UFUNCTION()
	void OnHit(AActor* otherActor);
};
