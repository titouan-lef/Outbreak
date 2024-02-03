// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Projectile.h"
#include "Zombie.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"

#include "Math/TransformNonVectorized.h"
#include <Kismet/KismetMathLibrary.h>

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

void AWeapon::StartFire()
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is nullptr"));
		return;
	}

	ShootEnable = true;
	Fire();
}

void AWeapon::StopFire()
{
	ShootEnable = false;
}

void AWeapon::UnlimitAmmo(bool enable)
{
	AmmoUnlimited = enable;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = InitialAmmo;
}

void AWeapon::OnHit(AProjectile* projectile, AActor* otherActor)
{
	AZombie* zombie = Cast<AZombie>(otherActor);
	if (zombie)
		zombie->TakeDamages(Damage);

	UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *otherActor->GetName());
	projectile->Destroy();
}

void AWeapon::Fire()
{
	if (CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ammo"));
		return;
	}

	if (!AmmoUnlimited)
		CurrentAmmo--;

	AProjectile* newProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Muzzle->GetComponentTransform());
	newProjectile->OnHit.AddDynamic(this, &AWeapon::OnHit);

	if (BulletByShoot > 1)
	{
		for (int i = 1; i < BulletByShoot; i++)
		{
			float pitch = UKismetMathLibrary::RandomFloatInRange(-BulletDispartion, BulletDispartion);
			pitch += Muzzle->GetComponentRotation().Pitch;
			float yaw = UKismetMathLibrary::RandomFloatInRange(-BulletDispartion, BulletDispartion);
			yaw += Muzzle->GetComponentRotation().Yaw;

			newProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, FTransform(FRotator(pitch, yaw, 0), Muzzle->GetComponentLocation(), Muzzle->GetComponentScale()));
			newProjectile->OnHit.AddDynamic(this, &AWeapon::OnHit);
		}
	}
	
	if (AutoShoot && ShootEnable)
	{
		FTimerHandle timerHandle;
		GetWorldTimerManager().SetTimer(timerHandle, this, &AWeapon::Fire, 0.1f, false);
	}
}

void AWeapon::AddAmmo()
{
	CurrentAmmo += InitialAmmo;
	if (CurrentAmmo > MaxAmmo)
		CurrentAmmo = MaxAmmo;
}

FString AWeapon::GetName()
{
	return Name;
}

int AWeapon::GetCurrentAmmo()
{
	return CurrentAmmo;
}
