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
	
	UPROPERTY(BlueprintReadOnly, Category = "GameData")
	int32 LivesRemaining;

	UPROPERTY(BlueprintReadOnly, Category = "GameData")
	float Score;

	UPROPERTY(BlueprintReadOnly, Category = "GameData")
	int32 CheckpointCounter;

	UPROPERTY(BlueprintReadOnly, Category = "GameData")
	FVector LastCheckpointLocation;

	void PlayerFell();
	void UpdateCheckpoint(const FVector& NewCheckpointTransform);
};
