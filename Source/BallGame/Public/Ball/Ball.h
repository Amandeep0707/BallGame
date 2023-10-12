// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "Ball.generated.h"

class UAudioComponent;
class UInputMappingContext;
class UInputAction;
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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Read Only Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ball Parameters")
	float MaxDesiredVelocity = 10.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball Paramaters")
	float DistanceTravelled;

	UPROPERTY(BlueprintReadOnly)
	bool bIsGameOver = false;
	

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputAction* IA_Pause;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);

	void Pause(const FInputActionValue& Value);

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

	
	UPROPERTY(EditAnywhere, Category = "Ball Audio Parameters")
	float HitSoundThreshold = 10.f;

	UPROPERTY(EditAnywhere, Category = "Ball Parameters")
	float ForceMultiplier = 7500.f;

	UPROPERTY(EditAnywhere, Category = "Ball Parameters")
	float JumpForce = 500000.f;

	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	float DefaultZoom = 35.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	float MaxVelocityZoom = 18.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Parameters")
	float CameraZoomInterpSpeed = 1.f;
	

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

	bool FloorTrace(FVector InputLocation);
};
