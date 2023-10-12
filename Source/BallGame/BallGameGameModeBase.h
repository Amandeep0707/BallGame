// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BallGameGameModeBase.generated.h"

class ABall;

UENUM()
enum EBallPhysicalMaterial : uint8
{
	MetalMaterial,
	WoodMaterial,
	PaperMaterial
};

UCLASS()
class BALLGAME_API ABallGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ABallGameGameModeBase();
	virtual void BeginPlay() override;

	//Read Only Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	ABall* BP_Ball;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	TEnumAsByte<EBallPhysicalMaterial> BallMaterialType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 LivesRemaining;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	float Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 CheckpointCounter;

	UFUNCTION()
	void GameOver();
};
