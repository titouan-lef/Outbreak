// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"
#include "Weapon.h"
#include "RunCharacter.h"
#include <Kismet/KismetMathLibrary.h>

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

	if (WeaponClasses.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No WeaponClasses"));
		return;
	}
	
	int randomIndex = UKismetMathLibrary::RandomIntegerInRange(0, WeaponClasses.Num() - 1);
	if (!WeaponClasses[randomIndex])
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponClasses[%d] is nullptr"), randomIndex);
		return;
	}

	AWeapon* newWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClasses[randomIndex]);
	if (!newWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("newWeapon null"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Weapon pickup succeeded"));

	runCharacter->GetNewWeapon(newWeapon);
}
