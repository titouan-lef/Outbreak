// Fill out your copyright notice in the Description page of Project Settings.


#include "Fire.h"
#include "Damageable.h"
#include "RunCharacter.h"

#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>

AFire::AFire()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AFire::Destroyed()
{
	Super::Destroyed();

	UE_LOG(LogTemp, Warning, TEXT("Fire destroyed"));

	for (FTimerHandle timerHandle : TimerHandles)
		GetWorldTimerManager().ClearTimer(timerHandle);
}

void AFire::ActiveEffect(ACharacter* character)
{
	Super::ActiveEffect(character);

	IDamageable* damageable = Cast<IDamageable>(character);

	if (!damageable)
		return;

	ARunCharacter* runCharacter = Cast<ARunCharacter>(character);
	if (runCharacter)
		UGameplayStatics::PlaySoundAtLocation(this, Burn, runCharacter->GetActorLocation());

	for (int i = 1; i <= NbRepetition; i++)
	{
		FTimerHandle timerHandle;
		GetWorld()->GetTimerManager().SetTimer(timerHandle, [damageable, runCharacter, this]() {
			damageable->TakeDamages(FireDamage);
			if (runCharacter)
				UGameplayStatics::PlaySoundAtLocation(this, BurnDamage, runCharacter->GetActorLocation());
		}, DamageDelay * i, false);
		TimerHandles.Add(timerHandle);
	}
}
