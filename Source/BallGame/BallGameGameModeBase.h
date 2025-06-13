// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BallGameGameModeBase.generated.h"

class ABallGameHUD;
class ABall;
class ABallPlayerController;

UCLASS()
class BALLGAME_API ABallGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ABallGameGameModeBase();
	virtual void BeginPlay() override;

	// --- Getter Functions ---
	FORCEINLINE int32 GetCurrentPlayerLives() const { return StartingPlayerLives; }

	
	UPROPERTY(BlueprintReadOnly, Category = "GameData")
	TObjectPtr<ABall> CachedBallRef;

	UPROPERTY(BlueprintReadOnly, Category = "GameData")
	TObjectPtr<ABallGameHUD> CachedHUDRef;

	UPROPERTY(BlueprintReadOnly, Category = "GameData")
	TObjectPtr<ABallPlayerController> CachedPlayerControllerRef;

	UPROPERTY(BlueprintReadOnly, Category = "GameData")
	FVector LastCheckpointLocation;

	void PlayerFell();
	void UpdateCheckpoint(const FVector& NewCheckpointTransform);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 StartingPlayerLives = 3;

	int32 CurrentPlayerLives;

	UFUNCTION(BlueprintCallable, Category = "Game Mode|References")
	ABall* GetBallPawn();

	UFUNCTION(BlueprintCallable, Category = "Game Mode|References")
	ABallGameHUD* GetBallGameHUD();

	UFUNCTION(BlueprintCallable, Category = "Game Mode|References")
	ABallPlayerController* GetBallPlayerController();

	void GameOver();
	void LevelComplete();
};
