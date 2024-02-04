// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "HealthComponent.h"
#include "RunCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Runtime/AIModule/Classes/AIController.h>
#include <Kismet/GameplayStatics.h>

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
		ControllerZombie->StopMovement();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FTimerHandle timerHandleDestroy;
		GetWorldTimerManager().SetTimer(timerHandleDestroy, this, &AZombie::Die, 2, false);
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
		ControllerZombie->MoveToActor(runCharacter);
}

void AZombie::InitControllerZombie()
{
	ControllerZombie = Cast<AAIController>(GetController());
	if (!ControllerZombie)
		UE_LOG(LogTemp, Warning, TEXT("Zombie has no AIController"));
}

void AZombie::Attack(ARunCharacter* runCharacter)
{
	if (!HealthComponent->IsDead() && CanAttack && !runCharacter->HealthComponent->IsDead())
	{
		CanAttack = false;
		runCharacter->TakeDamages(Damage);
		IsAttacking = true;

		if (AttackSound)
			UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());

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

void AZombie::Die()
{
	Destroy();
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
		Attack(runCharacter);
}

void AZombie::OnAttackDetectionOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ARunCharacter* runCharacter = Cast<ARunCharacter>(OtherActor);
	if (runCharacter)
		FollowPlayer(runCharacter);
}
