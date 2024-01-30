// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "HealthComponent.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

void AZombie::TakeDamage(float damage)
{
	HealthComponent->ChangeLife(-damage);

	if (HealthComponent->IsDead())
	{
		UE_LOG(LogTemp, Warning, TEXT("Zombie is dead"));
		Destroy();
	}
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

