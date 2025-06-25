// Copyright Epic Games, Inc. All Rights Reserved.


#include "BallGameGameModeBase.h"
#include "Ball/Ball.h"
#include "Ball/BallPlayerController.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/BallGameHUD.h"
#include "World/BallPlayerState.h"

ABallGameGameModeBase::ABallGameGameModeBase(): LastCheckpointLocation(FVector::ZeroVector)
{
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultPawnClass = ABall::StaticClass();
	PlayerControllerClass = ABallPlayerController::StaticClass();
	PlayerStateClass = ABallPlayerState::StaticClass();
}

void ABallGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	bIsScoreTicking = true;
	ScoreTickTimer = 0.0f;
	CurrentPlayerLives = StartingPlayerLives;
	
	if (GetBallPawn())
	{
		LastCheckpointLocation = CachedBallRef->GetActorLocation();
	}

	GetWorld()->Exec(GetWorld(), TEXT("r.ScreenPercentage 100"));
	GetWorld()->Exec(GetWorld(), TEXT("r.VSync 1"));
}

void ABallGameGameModeBase::ResumeGameTicker()
{
	bIsScoreTicking = true;
	ScoreTickTimer = 0.0f;
}

void ABallGameGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsScoreTicking)
	{
		ScoreTickTimer += DeltaSeconds;
		if (ScoreTickTimer >= ScoreTickInterval)
		{
			
			// Reset timer, accounting for any leftover time.
			ScoreTickTimer -= ScoreTickInterval;
            
			if (GetBallPlayerController())
			{
				if (ABallPlayerState* PS = CachedPlayerControllerRef->GetPlayerState<ABallPlayerState>())
				{
					PS->DecrementSessionScore(ScoreToDecrementPerTick);
				}
			}
		}
	}
}

void ABallGameGameModeBase::PlayerFell()
{
	// bIsScoreTicking = false; // Stop the clock while we respawn
	CurrentPlayerLives--;

	if (GetBallPlayerController())
	{
		if (ABallPlayerState* PS = CachedPlayerControllerRef->GetPlayerState<ABallPlayerState>())
		{
			// Apply penalty and reset session progress
			PS->AddToSessionScore(-LifeLostPenalty); // Subtract penalty
			PS->ResetSessionScore();
		}
	}

	// Update Player Remaining Lives in HUD
	if (GetBallGameHUD()) CachedHUDRef->PlayerLivesUpdate.Broadcast(CurrentPlayerLives);

	if (CurrentPlayerLives > 0)
	{
		if (GetBallPawn())
		{
			CachedBallRef->GetSimSphere()->SetSimulatePhysics(false);
			CachedBallRef->SetActorLocation(LastCheckpointLocation);
			CachedBallRef->GetSimSphere()->SetSimulatePhysics(true);
			CachedBallRef->SetIsGameOver(false);
		}
	}
	else
	{
		// GameOver();
		UE_LOG(LogTemp, Log, TEXT("Called GameOver"));
	}

	ResumeGameTicker(); // Start the clock again.
}

void ABallGameGameModeBase::HandleCheckpointReached(const FVector& RespawnLocation)
{
	// bIsScoreTicking = false; // Stop the clock! The player is safe.
	LastCheckpointLocation = RespawnLocation;

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (ABallPlayerState* PS = PC->GetPlayerState<ABallPlayerState>())
		{
			PS->BankSessionScore();
			ResumeGameTicker();
		}
	}
}

ABall* ABallGameGameModeBase::GetBallPawn()
{
	// 1. Check if we already have a valid, cached reference.
	if (CachedBallRef && !CachedBallRef.Get()->IsPendingKillPending())
	{
		return CachedBallRef;
	}

	// 2. If not, try to find it. The Pawn is owned by the PlayerController.
	if (const ABallPlayerController* PC = GetBallPlayerController())
	{
		CachedBallRef = Cast<ABall>(PC->GetPawn());
		return CachedBallRef;
	}

	// 3. If all else fails, return null.
	return nullptr;
}

ABallGameHUD* ABallGameGameModeBase::GetBallGameHUD()
{
	// 1. Check cache first.
	if (CachedHUDRef && !CachedHUDRef.Get()->IsPendingKillPending())
	{
		return CachedHUDRef;
	}

	// 2. If not, find it via the PlayerController.
	if (ABallPlayerController* PC = GetBallPlayerController())
	{
		CachedHUDRef = Cast<ABallGameHUD>(PC->GetHUD());
		return CachedHUDRef;
	}
    
	// 3. Return null if not found.
	return nullptr;
}

ABallPlayerController* ABallGameGameModeBase::GetBallPlayerController()
{
	if (CachedPlayerControllerRef)
	{
		return CachedPlayerControllerRef;
	}
    
	// PlayerController is the first thing that exists, so it's safe to get.
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		CachedPlayerControllerRef = Cast<ABallPlayerController>(PC);
		return CachedPlayerControllerRef;
	}
    
	return nullptr;
}

void ABallGameGameModeBase::GameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
	UGameplayStatics::OpenLevel(GetWorld(), "IntroLevel");
}

void ABallGameGameModeBase::LevelComplete()
{
	UE_LOG(LogTemp, Log, TEXT("LEVEL COMPLETE!"));
}
