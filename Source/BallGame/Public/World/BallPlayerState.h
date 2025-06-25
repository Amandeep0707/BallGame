#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BallPlayerState.generated.h"

// We'll broadcast the new total score whenever it changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewTotalScore);

UCLASS()
class BALLGAME_API ABallPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABallPlayerState();

	// Called by collectibles or other bonuses.
	void AddToSessionScore(int32 ScoreAmount);

	// Called by the GameMode to tick the score down.
	void DecrementSessionScore(int32 ScoreAmount);

	// Called by the GameMode when a checkpoint is reached.
	void BankSessionScore();

	// Called by the GameMode on player respawn.
	void ResetSessionScore();

	// BlueprintPure allows BP widgets to get the score without an execution pin.
	UFUNCTION(BlueprintPure, Category = "Player State")
	int32 GetTotalScore() const;

	UFUNCTION(BlueprintPure, Category = "Player State")
	int32 GetBankedScore() const { return BankedScore; }

	UFUNCTION(BlueprintPure, Category = "Player State")
	int32 GetSessionScore() const { return SessionScore; }

	// This is the delegate our UI will listen to for efficient updates.
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnScoreChanged OnScoreChanged;

private:
	/** Score that is "safe" and stored at the last checkpoint. */
	UPROPERTY()
	int32 BankedScore;

	/** Score accumulated since the last checkpoint. This is the value that ticks down. */
	UPROPERTY()
	int32 SessionScore;

	/** Helper function to notify listeners that the score has updated. */
	void BroadcastScoreChange();
};