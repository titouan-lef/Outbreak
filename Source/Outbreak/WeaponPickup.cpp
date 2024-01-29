// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"
#include "Weapon.h"
#include "RunCharacter.h"

void AWeaponPickup::OnGet(AActor* OtherActor)
{
	Super::OnGet(OtherActor);

	ARunCharacter* runCharacter = Cast<ARunCharacter>(OtherActor);
	if (!runCharacter)
		return;

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
