// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "RunCharacter.h"
#include "Weapon.h"

void AAmmoPickup::OnGet(ARunCharacter* runCharacter)
{
	Super::OnGet(runCharacter);

	AWeapon* weapon = runCharacter->GetWeapon();
	if (weapon)
		weapon->AddAmmo();
}
