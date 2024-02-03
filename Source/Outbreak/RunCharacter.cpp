// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacter.h"
#include "Weapon.h"
#include "HealthComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

void ARunCharacter::ChangeSpeedMovement(float delta)
{
	Speed += delta;
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

AWeapon* ARunCharacter::GetWeapon()
{
	return CurrentWeapon;
}

void ARunCharacter::Shoot()
{
	if (!CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon equipped"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Shoot"));
	CurrentWeapon->StartFire();
}

void ARunCharacter::StopShoot()
{
	if (!CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon equipped"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("StopShoot"));
	CurrentWeapon->StopFire();
}

void ARunCharacter::ChangeRamboMode(bool enable)
{
	if (enable)
		++RamboModeAccumulation;
	else
	--RamboModeAccumulation;

	if (RamboModeAccumulation == 0)
		OnRamboMode = false;
	else
		OnRamboMode = true;

	if (CurrentWeapon)
		CurrentWeapon->UnlimitAmmo(OnRamboMode);
}

FRotator ARunCharacter::GetCameraRotation()
{
	return Camera->GetComponentRotation();
}

void ARunCharacter::SetCameraRotation(FRotator cameraRotation)
{
	Camera->SetWorldRotation(cameraRotation);
	
	if (CurrentWeapon)
		CurrentWeapon->SetActorRotation(cameraRotation);
}

void ARunCharacter::TakeDamages(float damage)
{
	HealthComponent->ChangeLife(-damage);

	if (HealthComponent->IsDead())
	{
		GetMesh()->SetVisibility(false);
		OnDeath.Broadcast();
	}
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

		// Check if the new weapon is the same type as the current weapon
		if (newWeapon->GetClass() == CurrentWeapon->GetClass())
		{
			UE_LOG(LogTemp, Warning, TEXT("Same weapon type"));
			CurrentWeapon->AddAmmo();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Different weapon type"));
			CurrentWeapon->Destroy();
			CurrentWeapon = newWeapon;
			UE_LOG(LogTemp, Warning, TEXT("New weapon equipped"));

			// Attach the weapon to the Run Character
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			newWeapon->AttachToComponent(WeaponAttachment, AttachmentRules);
		}
	}
}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

