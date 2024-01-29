// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

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

	Fire();
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

	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Muzzle->GetComponentTransform());
	UE_LOG(LogTemp, Warning, TEXT("Fire !"));
}
