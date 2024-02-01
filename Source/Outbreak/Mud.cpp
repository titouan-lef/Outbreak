// Fill out your copyright notice in the Description page of Project Settings.


#include "Mud.h"
#include "SpeedMovement.h"

#include "GameFramework/Character.h"

AMud::AMud()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AMud::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->OnComponentEndOverlap.AddDynamic(this, &AMud::OnEndOverlap);
}

void AMud::ActiveEffect(ACharacter* character)
{
	Super::ActiveEffect(character);

	ISpeedMovement* speedMovement = Cast<ISpeedMovement>(character);

	if (speedMovement)
		speedMovement->ChangeSpeedMovement(-SlowSpeed);
}

void AMud::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType)
{
	ISpeedMovement* speedMovement = Cast<ISpeedMovement>(OtherActor);

	if (speedMovement)
		speedMovement->ChangeSpeedMovement(SlowSpeed);
}

