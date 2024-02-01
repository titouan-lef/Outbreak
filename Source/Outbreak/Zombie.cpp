// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "HealthComponent.h"
#include "RunCharacter.h"

#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Runtime/AIModule/Classes/AIController.h>

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	GetCapsuleComponent()->SetupAttachment(Scene);

	PlayerDetection = CreateDefaultSubobject<USphereComponent>("PlayerDetection");
	PlayerDetection->SetupAttachment(Scene);

	AttackDetection = CreateDefaultSubobject<USphereComponent>("AttackDetection");
	AttackDetection->SetupAttachment(Scene);

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

void AZombie::ChangeSpeedMovement(float delta)
{
	Speed += delta;
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();

	CanAttack = true;

	GetCharacterMovement()->MaxWalkSpeed = Speed;

	InitControllerZombie();

	PlayerDetection->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnPlayerDetectionOverlapBegin);

	AttackDetection->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnAttackDetectionOverlapBegin);
	AttackDetection->OnComponentEndOverlap.AddDynamic(this, &AZombie::OnAttackDetectionOverlapEnd);
}

void AZombie::FollowPlayer(ARunCharacter* runCharacter)
{
	if (!runCharacter->HealthComponent->IsDead())
	{
		ControllerZombie->MoveToActor(runCharacter);
		//ControllerZombie->MoveToLocation(FVector(0, 0, 0));
		UE_LOG(LogTemp, Warning, TEXT("FollowPlayer"));
	}
}

void AZombie::InitControllerZombie()
{
	ControllerZombie = Cast<AAIController>(GetController());
	if (!ControllerZombie)
		UE_LOG(LogTemp, Warning, TEXT("Zombie has no AIController"));
}

void AZombie::Attack(ARunCharacter* runCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("ATK"));
	if (CanAttack && !runCharacter->HealthComponent->IsDead())
	{
		UE_LOG(LogTemp, Warning, TEXT("ATK SUCCESS"));
		CanAttack = false;
		runCharacter->TakeDamages(Damage);

		FTimerHandle timerHandleAtk;
		FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &AZombie::RestartAttack, runCharacter);
		GetWorldTimerManager().SetTimer(timerHandleAtk, timerDelegate, atkSpeed, false);
	}
}

void AZombie::RestartAttack(ARunCharacter* runCharacter)
{
	CanAttack = true;

	if (AttackDetection->IsOverlappingActor(runCharacter))
		Attack(runCharacter);
}

void AZombie::OnPlayerDetectionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* runCharacter = Cast<ARunCharacter>(OtherActor);
	if (runCharacter)
		FollowPlayer(runCharacter);
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

void AZombie::OnAttackDetectionOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ARunCharacter* runCharacter = Cast<ARunCharacter>(OtherActor);
	if (runCharacter)
		FollowPlayer(runCharacter);
}
