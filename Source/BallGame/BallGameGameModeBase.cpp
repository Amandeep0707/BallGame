// Copyright Epic Games, Inc. All Rights Reserved.


#include "BallGameGameModeBase.h"
#include "Ball/Ball.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/BallGameHUD.h"

ABallGameGameModeBase::ABallGameGameModeBase()
{
	// Initialize Variables
}

void ABallGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentPlayerLives = StartingPlayerLives;
	
	if (const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		LastCheckpointLocation = PlayerPawn->GetActorLocation();
	}

	BP_Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	GameHUD = Cast<ABallGameHUD>(HUDClass);
	if (GameHUD)
	{
		GameHUD->PlayerLivesUpdate.Broadcast(CurrentPlayerLives);
	}
}

void ABallGameGameModeBase::PlayerFell()
{
	CurrentPlayerLives--;

	if (GameHUD)
	{
		GameHUD->PlayerLivesUpdate.Broadcast(CurrentPlayerLives);
	}

	if (CurrentPlayerLives > 0)
	{
		// Respawn logic (you already have this)
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
		{
			if(ABall* Ball = Cast<ABall>(PlayerPawn))
			{
				Ball->GetSimSphere()->SetSimulatePhysics(false);
				Ball->SetActorLocation(LastCheckpointLocation);
				Ball->GetSimSphere()->SetSimulatePhysics(true);
				Ball->SetIsGameOver(false);
			}
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

void ABallGameGameModeBase::GameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ABallGameGameModeBase::LevelComplete()
{
	UE_LOG(LogTemp, Log, TEXT("LEVEL COMPLETE!"));
}
