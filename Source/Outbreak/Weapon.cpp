// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Projectile.h"
#include "Zombie.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"

AWeapon::AWeapon()
{	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(Scene);

	Muzzle = CreateDefaultSubobject<UArrowComponent>("Muzzle");
	Muzzle->SetupAttachment(SkeletalMesh);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = InitialAmmo;
}

void AWeapon::OnHit(AActor* otherActor)
{
	AZombie* zombie = Cast<AZombie>(otherActor);
	if (zombie)
		zombie->TakeDamage(Damage);
}

void AWeapon::Fire()
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is nullptr"));
		return;
	}

	if (CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ammo"));
		return;
	}

	CurrentAmmo--;

	AProjectile* newProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Muzzle->GetComponentTransform());
	newProjectile->OnHit.AddDynamic(this, &AWeapon::OnHit);
}

FString AWeapon::GetName()
{
	return Name;
}

int AWeapon::GetCurrentAmmo()
{
	return CurrentAmmo;
}
