// Fill out your copyright notice in the Description page of Project Settings.


#include "StimpackPickup.h"
#include "RunCharacter.h"

void AStimpackPickup::OnGet(ARunCharacter* runCharacter)
{
	Super::OnGet(runCharacter);

	runCharacter->ChangeSpeedMovement(StimpackSpeed);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, [runCharacter, this]() { runCharacter->ChangeSpeedMovement(-StimpackSpeed); }, StimpackDuration, false);
}

