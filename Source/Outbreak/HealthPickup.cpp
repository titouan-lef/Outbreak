// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "RunCharacter.h"
#include "HealthComponent.h"

void AHealthPickup::OnGet(ARunCharacter* runCharacter)
{
	Super::OnGet(runCharacter);

	runCharacter->HealthComponent->ChangeLife(HealthAmount);
}
