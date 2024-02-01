// Fill out your copyright notice in the Description page of Project Settings.


#include "Fire.h"
#include "Damageable.h"

#include "GameFramework/Character.h"

AFire::AFire()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AFire::ActiveEffect(ACharacter* character)
{
	Super::ActiveEffect(character);

	IDamageable* damageable = Cast<IDamageable>(character);

	if (!damageable)
		return;

	for (int i = 1; i <= NbRepetition; i++)
	{
		FTimerHandle timerHandle;
		GetWorld()->GetTimerManager().SetTimer(timerHandle, [damageable, this]() { damageable->TakeDamages(FireDamage); }, DamageDelay * i, false);
	}
}
