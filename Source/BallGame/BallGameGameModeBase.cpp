// Copyright Epic Games, Inc. All Rights Reserved.


#include "BallGameGameModeBase.h"
#include "Ball/Ball.h"
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

	if(ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		BP_Ball = Ball;
	}
}

void ABallGameGameModeBase::GameOver()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestingLevel"));
}
