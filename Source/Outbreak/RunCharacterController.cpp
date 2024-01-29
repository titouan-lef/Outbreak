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
	InputComponent->BindAxis("MoveLR", this, &ARunCharacterController::MoveLR);
	InputComponent->BindAxis("LookLR", this, &ARunCharacterController::LookLR);
	InputComponent->BindAxis("LookUD", this, &ARunCharacterController::LookUD);

	// Actions
	InputComponent->BindAction("Jump", IE_Pressed, this, &ARunCharacterController::Jump);
	InputComponent->BindAction("Shoot", IE_Pressed, this, &ARunCharacterController::Shoot);
}

void ARunCharacterController::MoveLR(float scale)
{
	RunCharacter->AddMovementInput(RunCharacter->GetActorRightVector() * scale);
}

void ARunCharacterController::LookLR(float scale)
{
	FRotator rotation = RunCharacter->GetCameraRotation();
	rotation.Yaw += scale;

	if (rotation.Yaw > maxRotationLR)
		rotation.Yaw = maxRotationLR;
	else if (rotation.Yaw < -maxRotationLR)
		rotation.Yaw = -maxRotationLR;

	RunCharacter->SetCameraRotation(rotation);
}

void ARunCharacterController::LookUD(float scale)
{
	FRotator rotation = RunCharacter->GetCameraRotation();
	rotation.Pitch += scale;

	if (rotation.Pitch > maxRotationUD)
		rotation.Pitch = maxRotationUD;
	else if (rotation.Pitch < -maxRotationUD)
		rotation.Pitch = -maxRotationUD;

	RunCharacter->SetCameraRotation(rotation);
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

