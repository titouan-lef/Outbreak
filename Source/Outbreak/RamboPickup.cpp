// Fill out your copyright notice in the Description page of Project Settings.


#include "RamboPickup.h"
#include "RunCharacter.h"

void ARamboPickup::OnGet(ARunCharacter* runCharacter)
{
	Super::OnGet(runCharacter);

	runCharacter->ChangeRamboMode(true);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, [runCharacter, this]() { runCharacter->ChangeRamboMode(false); }, RamboModeDuration, false);
}

