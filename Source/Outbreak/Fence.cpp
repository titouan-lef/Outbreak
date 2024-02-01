// Fill out your copyright notice in the Description page of Project Settings.


#include "Fence.h"

#include "Components/BoxComponent.h"

AFence::AFence()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BlockCollision = CreateDefaultSubobject<UBoxComponent>("BoxOverlap");
	BlockCollision->SetupAttachment(Scene);
}

void AFence::ActiveEffect(ACharacter* character)
{
	Super::ActiveEffect(character);
	UE_LOG(LogTemp, Warning, TEXT("Fence"));
}
