// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BallTypes.generated.h"

UENUM(BlueprintType)
enum class EBallMaterial : uint8
{
	Wood,
	Stone,
	Paper
};

USTRUCT(BlueprintType)
struct FBallMaterialProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material Properties")
	FString MaterialName = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material Properties")
	UStaticMesh* BallMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material Properties")
	UPhysicalMaterial* PhysicsMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material Properties")
	float MassScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material Properties")
	float LinearDamping = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material Properties")
	float AngularDamping = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material Properties")
	float ForceMultiplier = 500000.f;
};

UCLASS()
class BALLGAME_API UBallMaterialDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball Material")
	FBallMaterialProperties Properties;
};