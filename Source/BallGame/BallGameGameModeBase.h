// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BallGameGameModeBase.generated.h"

class ABallGameHUD;
class ABall;

UCLASS()
class BALLGAME_API ABallGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ABallGameGameModeBase();
	virtual void BeginPlay() override;

	// --- Getter Functions ---
	FORCEINLINE int32 GetCurrentPlayerLives() const { return StartingPlayerLives; }

	//Read Only Variables
	UPROPERTY(BlueprintReadOnly, Category = "GameData")
	ABall* BP_Ball;

	UPROPERTY(BlueprintReadOnly, Category = "GameData")
	ABallGameHUD* GameHUD;

	UPROPERTY(BlueprintReadOnly, Category = "GameData")
	FVector LastCheckpointLocation;

	void PlayerFell();
	void UpdateCheckpoint(const FVector& NewCheckpointTransform);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 StartingPlayerLives = 3;

	int32 CurrentPlayerLives;

	void GameOver();
	void LevelComplete();
};
