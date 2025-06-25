// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectible.generated.h"

class USphereComponent;
class URotatingMovementComponent;

UCLASS()
class BALLGAME_API ACollectible : public AActor
{
	GENERATED_BODY()

public:
	ACollectible();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CollectibleMesh;

	// Use a sphere for simple, efficient collision detection.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* TriggerSphere;
    
	// This component makes the actor spin automatically. Purely visual!
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URotatingMovementComponent* RotatingMovement;
	
	// --- CONFIGURATION ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	int32 PointValue = 100;

	// --- EFFECTS ---

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class USoundCue* CollectedSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystem* CollectedVFX;

private:
	
};
