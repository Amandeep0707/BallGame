// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball/BallTypes.h"
#include "NonPlayableBall.generated.h"

class USphereComponent;

UCLASS()
class BALLGAME_API ANonPlayableBall : public AActor
{
	GENERATED_BODY()

public:
	ANonPlayableBall();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USphereComponent* SimSphere;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ball Parameters | Default")
	EBallMaterial DefaultMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Ball Parameters | Default")
	float BallRadius = 20.f;

	/** The map of all available material data assets */
	UPROPERTY(EditDefaultsOnly, Category = "Ball Parameters | Material")
	TMap<EBallMaterial, UBallMaterialDataAsset*> MaterialDataAssets;

	UPROPERTY(BlueprintReadOnly, Category="Ball Parameters | Material")
	EBallMaterial CurrentMaterial;

	UPROPERTY(BlueprintReadOnly, Category="Gameplay")
	FTimerHandle TraceTimer;

private:

	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void ChangeMaterial(EBallMaterial NewMaterial);

	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void ApplyMaterialProperties();

	void CheckFalling();
};
