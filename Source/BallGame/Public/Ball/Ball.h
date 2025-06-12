// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "Ball/BallTypes.h"
#include "Ball.generated.h"

class ABallGameHUD;
class UAudioComponent;
class USpringArmComponent;
class UCineCameraComponent;
class USphereComponent;
class UCurveFloat;
class ABallGameGameModeBase;

UCLASS()
class BALLGAME_API ABall : public APawn
{
	GENERATED_BODY()

public:
	
	ABall();
	virtual void Tick(float DeltaTime) override;
	
	// Getter Functions
	FORCEINLINE float GetMaxDesiredVelocity() const { return MaxDesiredVelocity; }
	FORCEINLINE float GetDistanceTravelled() const { return DistanceTravelled; }
	
	//Custom Functions
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USphereComponent* SimSphere;

	//Read Only Variables
	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly)
	bool bIsGameOver = false;

	UPROPERTY(BlueprintReadOnly)
	float DistanceTravelled;
	
private:

	UPROPERTY(VisibleDefaultsOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCineCameraComponent* Camera;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ball Parameters | GameData")
	ABallGameGameModeBase* GameMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Ball Parameters | Audio")
	UAudioComponent* RollAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Ball Parameters | Audio")
	UAudioComponent* HitAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Ball Parameters | Audio")
	UCurveFloat* RollSoundCurve;

	UPROPERTY(EditAnywhere, Category = "Ball Parameters | Audio")
	float HitSoundThreshold = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Ball Parameters | Default")
	float MaxDesiredVelocity = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ball Parameters | Default")
	float BallRadius = 20.f;

	UPROPERTY(EditAnywhere, Category = "Ball Parameters | Default")
	float ForceMultiplier = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Ball Parameters | Camera")
	float DefaultZoom = 35.0f;

	UPROPERTY(EditAnywhere, Category = "Ball Parameters | Camera")
	float MaxVelocityZoom = 16.0f;

	UPROPERTY(EditAnywhere, Category = "Ball Parameters | Camera")
	float CameraZoomInterpSpeed = 5.f;

	// The map of all available material data assets
	UPROPERTY(EditDefaultsOnly, Category = "Ball Parameters | Material")
	TMap<EBallMaterial, UBallMaterialDataAsset*> MaterialDataAssets;

	UPROPERTY(EditInstanceOnly, Category = "Ball Parameters | Material")
	EBallMaterial DefaultMaterial;

	UPROPERTY(VisibleInstanceOnly, Category = "Ball Parameters | Material")
	EBallMaterial CurrentMaterial;
	
	UFUNCTION(BlueprintCallable, Category = "Ball Parameters | Material")
	void ChangeMaterial(EBallMaterial NewMaterial);
	

	//Private Functions
	void SetCamFocus();
	void SetCamZoom();
	void GetTravelledDistance(float Time);
	void SetRollAudioIntensity();
	void ApplyMaterialProperties();

	//Delegates
	UFUNCTION()
	void OnRollAudioFinished();

	UFUNCTION()
	void OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	bool FloorTrace();
};