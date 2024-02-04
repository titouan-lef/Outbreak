// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Projectile.h"
#include "Zombie.h"
#include "HealthComponent.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"

#include "Math/TransformNonVectorized.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

AWeapon::AWeapon()
{	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(Scene);
}

void AWeapon::StartFire(class UArrowComponent* muzzle)
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is nullptr"));
		return;
	}

	ShootEnable = true;
	Fire(muzzle);
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
	{
		zombie->TakeDamages(Damage);
		if (zombie->HealthComponent->IsDead())
			OnKill.Broadcast();
	}

	projectile->Destroy();
}

void AWeapon::Fire(class UArrowComponent* muzzle)
{
	if (CurrentAmmo <= 0)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EmptyGunSound, GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("No ammo"));
		return;
	}

	if (!AmmoUnlimited)
		CurrentAmmo--;

	AProjectile* newProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, muzzle->GetComponentTransform());
	newProjectile->OnHit.AddDynamic(this, &AWeapon::OnHit);

	UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation());

	if (BulletByShoot > 1)
	{
		for (int i = 1; i < BulletByShoot; i++)
		{
			float pitch = UKismetMathLibrary::RandomFloatInRange(-BulletDispartion, BulletDispartion);
			pitch += muzzle->GetComponentRotation().Pitch;
			float yaw = UKismetMathLibrary::RandomFloatInRange(-BulletDispartion, BulletDispartion);
			yaw += muzzle->GetComponentRotation().Yaw;

			newProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, FTransform(FRotator(pitch, yaw, 0), muzzle->GetComponentLocation(), muzzle->GetComponentScale()));
			newProjectile->OnHit.AddDynamic(this, &AWeapon::OnHit);
		}
	}
	
	if (AutoShoot && ShootEnable)
	{
		FTimerHandle timerHandle;
		FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &AWeapon::Fire, muzzle);
		GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, 0.1f, false);
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

void AWeapon::WeaponVisibility(bool visible)
{
	SkeletalMesh->SetVisibility(visible);
}
