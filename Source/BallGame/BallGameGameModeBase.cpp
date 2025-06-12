// Copyright Epic Games, Inc. All Rights Reserved.


#include "BallGameGameModeBase.h"
#include "Ball/Ball.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ABallGameGameModeBase::ABallGameGameModeBase()
{
	//Initialize Variables
	BallMaterialType = EBallPhysicalMaterial::MetalMaterial;
	LivesRemaining = 3;
	Score = 100.f;
	CheckpointCounter = 0;
	
}

void ABallGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		LastCheckpointTransform = PlayerPawn->GetActorTransform();
	}

	if(ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		BP_Ball = Ball;
	}
}

void ABallGameGameModeBase::PlayerFell()
{
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		// Disable physics, then teleport, then re-enable
		ABall* Ball = Cast<ABall>(PlayerPawn);
		if(Ball)
		{
			Ball->GetSimSphere()->SetSimulatePhysics(false);
			Ball->SetActorTransform(LastCheckpointTransform);
			Ball->GetSimSphere()->SetSimulatePhysics(true);
			Ball->bIsGameOver = false;
		}
	}
}

void ABallGameGameModeBase::UpdateCheckpoint(const FTransform& NewCheckpointTransform)
{
	LastCheckpointTransform = NewCheckpointTransform;
}
