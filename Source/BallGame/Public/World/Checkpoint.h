// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint.generated.h"

class UBoxComponent;

UCLASS()
class BALLGAME_API ACheckpoint : public AActor
{
	GENERATED_BODY()

public:
	ACheckpoint();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"), Category = "Components")
	USceneComponent* Root;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USceneComponent* SnapPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	UBoxComponent* TriggerVolume;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
