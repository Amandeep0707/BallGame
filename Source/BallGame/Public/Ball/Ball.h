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

	UPROPERTY(BlueprintReadOnly)
	bool bIsGameOver = false;
	
	ABall();
	virtual void Tick(float DeltaTime) override;
	
	// Getter Functions
	FORCEINLINE float GetMaxDesiredVelocity() const { return MaxDesiredVelocity; }
	FORCEINLINE float GetDistanceTravelled() const { return DistanceTravelled; }
	FORCEINLINE USphereComponent* GetSimSphere() const { return SimSphere; }
	FORCEINLINE void SetIsGameOver(bool Value) { bIsGameOver = Value; }
	
	/** Returns true if the ball is currently in an automated movement sequence. */
	FORCEINLINE bool IsAutoPiloting() const { return bIsAutoPiloting; }
	
	//Custom Functions
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Ball Parameters | Material")
	void ChangeMaterial(EBallMaterial NewMaterial);

	/**
	* @brief Initiates an automated movement sequence to a target location.
	* Disables player input, moves the ball, pauses, and then calls a completion action.
	* @param TargetLocation The world-space location to move the ball to.
	* @param Requester The actor that initiated this request (e.g., the checkpoint or material swapper).
	*/
	void StartAutoPilot(const FVector& TargetLocation, AActor* Requester = nullptr);

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
	float DistanceTravelled;

	UPROPERTY(BlueprintReadOnly)
	float ForceMultiplier = 1.f;

	/** Called from Tick() to handle the physics-based movement towards the autopilot target. */
	void UpdateAutoPilot(float DeltaTime);

	/** Called by a timer after the ball has settled at the target location. Releases control back to the player. */
	void StopAutoPilot();
	
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

	UPROPERTY(EditInstanceOnly, Category = "Ball Parameters | Default")
	EBallMaterial DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Ball Parameters | Camera")
	float DefaultZoom = 35.0f;

	UPROPERTY(EditAnywhere, Category = "Ball Parameters | Camera")
	float MaxVelocityZoom = 16.0f;

	UPROPERTY(EditAnywhere, Category = "Ball Parameters | Camera")
	float CameraZoomInterpSpeed = 5.f;

	/** The map of all available material data assets */
	UPROPERTY(EditDefaultsOnly, Category = "Ball Parameters | Material")
	TMap<EBallMaterial, UBallMaterialDataAsset*> MaterialDataAssets;

	/** True if the ball is currently in an automated movement sequence. */
	UPROPERTY(VisibleInstanceOnly, Category = "Ball Parameters | State")
	bool bIsAutoPiloting = false;

	/** If true, the player can provide move/look input. Set to false during autopilot. */
	UPROPERTY(VisibleInstanceOnly, Category = "Ball Parameters | State")
	bool bPlayerInputEnabled = true;

	/** The world-space location the ball is trying to reach during autopilot. */
	FVector AutoPilotTargetLocation;

	/** A handle for the timer that waits for VFX/settling before releasing control. */
	FTimerHandle SettleTimerHandle;

	/** A weak pointer to the actor that requested the autopilot, so we can perform context-specific actions on completion. */
	TWeakObjectPtr<AActor> AutoPilotRequester;

	// --- AUTOPILOT PHYSICS ---
    
	/** The strength of the force pulling the ball towards the target. */
	UPROPERTY(EditAnywhere, Category="Ball Parameters | Autopilot")
	float AutoPilotStrengthMultiplier = 10.0f;

	/** The strength of the damping force to prevent overshooting. */
	UPROPERTY(EditAnywhere, Category="Ball Parameters | Autopilot")
	float AutoPilotDampingMultiplier = 3.0f;

	UPROPERTY()
	EBallMaterial CurrentMaterial;
	

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
