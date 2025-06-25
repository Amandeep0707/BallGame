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
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	// Called by the Ball after its autopilot finishes, to resume the game's clock.
	void ResumeGameTicker();

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
	void HandleCheckpointReached(const FVector& RespawnLocation);

protected:

	// --- SCORING RULES ---
    
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules|Scoring")
	float ScoreTickInterval = 1.0f; // How often (in seconds) the score ticks down.

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules|Scoring")
	int32 ScoreToDecrementPerTick = 1; // How much score is lost per tick.
    
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules|Scoring")
	int32 LifeLostPenalty = 200; // Score penalty for falling.

private:

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 StartingPlayerLives = 3;

	int32 CurrentPlayerLives = 3;
	
	float ScoreTickTimer;
	bool bIsScoreTicking;

	UFUNCTION(BlueprintCallable, Category = "Game Mode|References")
	ABall* GetBallPawn();

	UFUNCTION(BlueprintCallable, Category = "Game Mode|References")
	ABallGameHUD* GetBallGameHUD();

	UFUNCTION(BlueprintCallable, Category = "Game Mode|References")
	ABallPlayerController* GetBallPlayerController();

	void GameOver();
	void LevelComplete();
};
