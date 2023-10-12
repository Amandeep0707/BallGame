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

	if(GEngine)
	{
		FString AllData;
		AllData.Append(TEXT("Ball Physical Material: "));
		AllData.Append(FString::Printf(TEXT("%d"), static_cast<int32>(BallMaterialType)));
		AllData.Append(TEXT("\nLives Remaining: "));
		AllData.Append(FString::Printf(TEXT("%d"), LivesRemaining));
		AllData.Append(TEXT("\nScore: "));
		AllData.Append(FString::Printf(TEXT("%f"), Score));
		AllData.Append(TEXT("\nCheckpoint Counter: "));
		AllData.Append(FString::Printf(TEXT("%d"), CheckpointCounter));

		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Cyan, AllData);
		GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Magenta, BP_Ball->GetName());
	}
}

void ABallGameGameModeBase::GameOver()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestingLevel"));
}
