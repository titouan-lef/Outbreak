// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RunCharacterController.generated.h"

class ARunCharacter;

/**
 * 
 */
UCLASS()
class OUTBREAK_API ARunCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARunCharacterController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ARunCharacter* RunCharacter;

	// INPUTS
	UFUNCTION()
	void SetupInputComponent() override;

	UFUNCTION()
	void MoveLR(float scale);

	UFUNCTION()
	void LookLR(float scale);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxRotationLR = 45;

	UFUNCTION()
	void LookUD(float scale);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxRotationUD = 30;

	UFUNCTION()
	void Jump();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* JumpSound;

	UFUNCTION()
	void Shoot();

	UFUNCTION()
	void StopShoot();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void InitRunCharacter();

	UFUNCTION()
	void OnDeath();
};
