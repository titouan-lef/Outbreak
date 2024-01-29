// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacter.h"
#include "Weapon.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ARunCharacter::ARunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// INIT COMPONENTS
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	WeaponAttachment = CreateDefaultSubobject<UArrowComponent>("WeaponAttachment");
	WeaponAttachment->SetupAttachment(GetMesh());
}

void ARunCharacter::Shoot()
{
	if (!CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon equipped"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Shoot"));
	CurrentWeapon->Fire();
}

void ARunCharacter::GetNewWeapon(AWeapon* newWeapon)
{
	if (!CurrentWeapon)// If no weapon equipped
	{
		CurrentWeapon = newWeapon;
		UE_LOG(LogTemp, Warning, TEXT("New weapon equipped"));

		// Attach the weapon to the Run Character
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		newWeapon->AttachToComponent(WeaponAttachment, AttachmentRules);

		//newWeapon->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon already equipped"));
	}
}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

