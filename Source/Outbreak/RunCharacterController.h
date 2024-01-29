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
	void MoveRight(float scale);

	UFUNCTION()
	void Jump();

	UFUNCTION()
	void Shoot();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void InitRunCharacter();
};
