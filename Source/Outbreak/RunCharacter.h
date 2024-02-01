// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpeedMovement.h"
#include "Damageable.h"
#include "RunCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathSignature);

class AWeapon;

UCLASS()
class OUTBREAK_API ARunCharacter : public ACharacter, public ISpeedMovement, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARunCharacter();

	virtual void ChangeSpeedMovement(float delta) override;
	virtual void TakeDamages(float damage) override;

	// EVENT DISPATCHERS
	UPROPERTY(BlueprintAssignable)
	FDeathSignature OnDeath;

	// PUBLIC FUNCTIONS
	UFUNCTION(BlueprintCallable)
	AWeapon* GetWeapon();

	UFUNCTION()
	void Shoot();

	UFUNCTION()
	FRotator GetCameraRotation();

	UFUNCTION()
	void GetNewWeapon(class AWeapon* newWeapon);

	UFUNCTION()
	void SetCameraRotation(FRotator cameraRotation);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UHealthComponent* HealthComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* Camera;

	// WEAPON
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* CurrentWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UArrowComponent* WeaponAttachment;

	// SPEED MOVEMENT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpeedMovement")
	float Speed = 100;
};
