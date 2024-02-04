// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UHealthComponent::ChangeLife(float delta)
{
	CurrentHealth += delta;

	if (CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;
	else if (CurrentHealth < 0)
		CurrentHealth = 0;
}

bool UHealthComponent::IsDead()
{
	return CurrentHealth <= 0;
}

float UHealthComponent::GetLife()
{
	return CurrentHealth;
}

