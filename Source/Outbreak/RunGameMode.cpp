// Fill out your copyright notice in the Description page of Project Settings.


#include "RunGameMode.h"
#include "Tile.h"
#include "RunCharacter.h"

#include "Blueprint/UserWidget.h"
#include <Kismet/GameplayStatics.h>

ARunGameMode::ARunGameMode()
{
	// Init LastTileTransform for the first tile
	LastTileTransform.SetLocation(FVector::Zero());
	LastTileTransform.SetRotation(FQuat::Identity);
	LastTileTransform.SetScale3D(FVector(1, 1, 1));
}

void ARunGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay RunGameMode"));

	//ObserverRunCharacter();

	for (int i = 0; i < NbInitialTiles; i++)
		AddTile();

	SetUpWidget();
}

void ARunGameMode::AddTile()
{
	if (!TileClass) return;

	ATile* newTile = GetWorld()->SpawnActor<ATile>(TileClass, LastTileTransform);
	LastTileTransform = newTile->GetTransformAttachPoint();

	newTile->OnExited.AddDynamic(this, &ARunGameMode::OnExited);
}

void ARunGameMode::RemoveTile(ATile* tile)
{
	tile->Destroy();
}

void ARunGameMode::OnExited(ATile* tile)
{
	AddTile();

	// Delay before destroying the tile
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &ARunGameMode::RemoveTile, tile);
	GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, delayRemoveTile, false);
}

void ARunGameMode::SetUpWidget()
{
	if (!HUDWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUDWidget is null"));
		return;
	}

	UUserWidget* inGameScreen = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(), 0), HUDWidget);
	if (!inGameScreen)
	{
		UE_LOG(LogTemp, Warning, TEXT("inGameScreen is null"));
		return;
	}
	inGameScreen->AddToViewport();
}
