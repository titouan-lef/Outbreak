// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpeedMovement.h"
#include "Damageable.h"
#include "NotStackingActor.h"
#include "Zombie.generated.h"

UCLASS()
class OUTBREAK_API AZombie : public ACharacter, public ISpeedMovement, public IDamageable, public INotStackingActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

	virtual void ChangeSpeedMovement(float delta) override;
	virtual void TakeDamages(float damage) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* PlayerDetection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* AttackDetection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UHealthComponent* HealthComponent;

	// FOLOW PLAYER
	UFUNCTION()
	void FollowPlayer(ARunCharacter* runCharacter);

	// ATTACK
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float atkSpeed = 2;

	// SPEED MOVEMENT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpeedMovement")
	float Speed = 100;

	// EVENTS
	UFUNCTION()
	void OnPlayerDetectionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackDetectionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackDetectionOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	// FOLOW PLAYER
	class AAIController* ControllerZombie = nullptr;

	void InitControllerZombie();

	// ATTACK
	bool CanAttack = false;

	void Attack(ARunCharacter* runCharacter);
	void RestartAttack(ARunCharacter* runCharacter);

	// DEATH
	void Die();
};
