// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"
#include "Weapon.h"
#include "RunCharacter.h"

AWeaponPickup::AWeaponPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh2 = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh2");
	StaticMesh2->SetupAttachment(StaticMesh);
}

void AWeaponPickup::OnGet(ARunCharacter* runCharacter)
{
	Super::OnGet(runCharacter);

	if (!WeaponClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponClass null"));
		return;
	}

	AWeapon* newWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	if (!newWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("newWeapon null"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Weapon pickup succeeded"));

	runCharacter->GetNewWeapon(newWeapon);
}
