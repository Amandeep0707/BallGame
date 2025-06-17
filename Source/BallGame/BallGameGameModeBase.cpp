// Copyright Epic Games, Inc. All Rights Reserved.


#include "BallGameGameModeBase.h"
#include "Ball/Ball.h"
#include "Ball/BallPlayerController.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/BallGameHUD.h"

ABallGameGameModeBase::ABallGameGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> Player(
		TEXT("/Game/Blueprints/Ball/BP_Ball.BP_Ball"));
	if (Player.Class != nullptr)
	{
		DefaultPawnClass = Player.Class;
	}
	PlayerControllerClass = APlayerController::StaticClass();
}

void ABallGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentPlayerLives = StartingPlayerLives;
	
	if (const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		LastCheckpointLocation = PlayerPawn->GetActorLocation();
	}
}

void ABallGameGameModeBase::PlayerFell()
{
	CurrentPlayerLives--;

	// Update Player Remaining Lives in HUD
	if (GetBallGameHUD()) CachedHUDRef->PlayerLivesUpdate.Broadcast(CurrentPlayerLives);CachedHUDRef->PlayerLivesUpdate.Broadcast(CurrentPlayerLives);

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
		GameOver();
	}
}

void ABallGameGameModeBase::UpdateCheckpoint(const FVector& NewCheckpointTransform)
{
	LastCheckpointLocation = NewCheckpointTransform;
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
