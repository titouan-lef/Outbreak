// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "HealthComponent.h"
#include "RunCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include <Runtime/AIModule/Classes/AIController.h>

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PlayerDetection = CreateDefaultSubobject<USphereComponent>("PlayerDetection");
	PlayerDetection->SetupAttachment(GetCapsuleComponent());

	AttackDetection = CreateDefaultSubobject<USphereComponent>("AttackDetection");
	AttackDetection->SetupAttachment(GetCapsuleComponent());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

void AZombie::TakeDamages(float damage)
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

	canAttack = true;
	
	PlayerDetection->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnPlayerDetectionOverlapBegin);

	AttackDetection->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnAttackDetectionOverlapBegin);
}

void AZombie::FollowPlayer(AAIController* controllerZombie, ARunCharacter* runCharacter)
{
	if (!runCharacter->HealthComponent->IsDead())
	{
		controllerZombie->MoveToActor(runCharacter);
		FTimerHandle timerHandle;
		FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &AZombie::FollowPlayer, controllerZombie, runCharacter);
		GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, 1, false);
	}
}

void AZombie::Attack(ARunCharacter* runCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("ATK"));
	if (canAttack && !runCharacter->HealthComponent->IsDead())
	{
		UE_LOG(LogTemp, Warning, TEXT("ATK SUCCESS"));
		canAttack = false;
		runCharacter->TakeDamages(Damage);

		FTimerHandle timerHandleAtk;
		FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &AZombie::RestartAttack, runCharacter);
		GetWorldTimerManager().SetTimer(timerHandleAtk, timerDelegate, atkSpeed, false);
	}
}

void AZombie::RestartAttack(ARunCharacter* runCharacter)
{
	canAttack = true;
	if (AttackDetection->IsOverlappingActor(runCharacter))
		Attack(runCharacter);
}

void AZombie::OnPlayerDetectionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Zombie detected player"));
	AAIController* controllerZombie = Cast<AAIController>(GetController());
	if (!controllerZombie)
	{
		UE_LOG(LogTemp, Warning, TEXT("Zombie has no AIController"));
		return;
	}

	ARunCharacter* runCharacter = Cast<ARunCharacter>(OtherActor);
	if (runCharacter)
		FollowPlayer(controllerZombie, runCharacter);
}

void AZombie::OnAttackDetectionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* runCharacter = Cast<ARunCharacter>(OtherActor);
	if (runCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Zombie attacked player"));
		Attack(runCharacter);
	}
		
}
