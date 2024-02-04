// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RunGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAK_API ARunGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARunGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// TILE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	int NbInitialTiles = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	int NbLeadTiles = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	float delayRemoveTile = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	class TSubclassOf<class ATile> TileClass;

	// PLAYER

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float delayReloadLevel = 4;

	UFUNCTION()
	void OnDeath();

	// WIDGET
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class TSubclassOf<class UUserWidget> HUDWidget;

private:
	// TILE
	UPROPERTY(VisibleAnywhere)
	FTransform LastTileTransform;

	UFUNCTION()
	void AddTile(bool fillTile);

	UFUNCTION()
	void RemoveTile(ATile* tile);

	UFUNCTION()
	void OnExited(class ATile* tile);

	// PLAYER
	UFUNCTION()
	void ObserverRunCharacter();

	UFUNCTION()
	void ReloadLevel();

	// WIDGET
	UFUNCTION()
	void SetUpWidget();
};
