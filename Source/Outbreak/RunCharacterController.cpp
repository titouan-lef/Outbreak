// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacterController.h"
#include "RunCharacter.h"

ARunCharacterController::ARunCharacterController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ARunCharacterController::BeginPlay()
{
	Super::BeginPlay();

	InitRunCharacter();
}

void ARunCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Axis
	InputComponent->BindAxis("MoveRight", this, &ARunCharacterController::MoveRight);

	// Actions
	InputComponent->BindAction("Jump", IE_Pressed, this, &ARunCharacterController::Jump);
	InputComponent->BindAction("Shoot", IE_Pressed, this, &ARunCharacterController::Shoot);
}

void ARunCharacterController::MoveRight(float scale)
{
	RunCharacter->AddMovementInput(RunCharacter->GetActorRightVector() * scale);
}

void ARunCharacterController::Jump()
{
	RunCharacter->Jump();
}

void ARunCharacterController::Shoot()
{
	RunCharacter->Shoot();
}

void ARunCharacterController::InitRunCharacter()
{
	RunCharacter = Cast<ARunCharacter>(Super::GetPawn());

	if (!RunCharacter)
		UE_LOG(LogTemp, Warning, TEXT("RunCharacter is null"));
}

// Called every frame
void ARunCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunCharacter->AddMovementInput(RunCharacter->GetActorForwardVector());
}

