// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
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

	//Read Only Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball Parameters")
	float MaxDesiredVelocity = 10.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball Paramaters")
	float DistanceTravelled;

	UPROPERTY(BlueprintReadOnly)
	bool bIsGameOver = false;

	//Custom Functions
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SimSphere;

	//Read Only Variables
	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling;
	
private:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCineCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "GameData")
	ABallGameGameModeBase* GameMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Ball Audio Parameters")
	UAudioComponent* RollAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Ball Audio Parameters")
	UAudioComponent* HitAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Ball Audio Parameters")
	UCurveFloat* RollSoundCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ball Audio Parameters")
	float HitSoundThreshold = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ball Parameters")
	float BallRadius = 20.f;

	UPROPERTY(EditAnywhere, Category = "Ball Parameters")
	float ForceMultiplier = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	float DefaultZoom = 35.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	float MaxVelocityZoom = 16.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	float CameraZoomInterpSpeed = 5.f;
	

	//Private Functions
	void SetCamFocus();
	void SetCamZoom();
	void GetTravelledDistance(float Time);
	void SetRollAudioIntensity();

	//Delegates
	UFUNCTION()
	void OnRollAudioFinished();

	UFUNCTION()
	void OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	bool FloorTrace();
};