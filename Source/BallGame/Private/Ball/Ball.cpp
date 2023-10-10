// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/Ball.h"

#include "CineCameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/AudioComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	SimSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SimSphere);
	SimSphere->SetSimulatePhysics(true);
	SimSphere->SetCollisionProfileName(TEXT("BlockAll"));
	SimSphere->SetLinearDamping(0.5f);
	SimSphere->SetAngularDamping(0.5f);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCineCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->CurrentFocalLength = 18.f;
	Camera->CurrentAperture = 0.8f;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
	BallMesh->SetupAttachment(SimSphere);

	RollAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("RollAudioComponent"));
	RollAudio->SetupAttachment(GetRootComponent());

	HitAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("HitAudioComponent"));
	HitAudio->SetupAttachment(GetRootComponent());
	
}

void ABall::BeginPlay()
{
	Super::BeginPlay();

	//Binding Delegates
	RollAudio->OnAudioFinished.AddDynamic(this, &ABall::OnRollAudioFinished);
	
	//Setting Distance travelled to 0 when the game starts
	DistanceTravelled = 0.f;

	//Create and Enable Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	//Enable HUD Widget
	if(GameplayHUD)
	{
		HUD = CreateWidget<UUserWidget>(GetWorld(), GameplayHUD);
		HUD->AddToViewport();
	}

	//Start Playing Roll Audio
	RollAudio->Play();
	
}
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetCamFocus();
	SetCamZoom();
	GetTravelledDistance(DeltaTime);
	SetRollAudioIntensity();

	FloorTrace(BallMesh->GetComponentLocation());
	
}

//Input Functions
void ABall::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.	Get<FVector2D>();

	//Get Transformed Movement for Force.
	const FMatrix RotMatrix = FRotationMatrix(GetController()->GetControlRotation());
	FVector LocalForce = RotMatrix.TransformVector(FVector(MovementVector.Y, MovementVector.X, 0.f)) * ForceMultiplier;
	LocalForce = LocalForce.GetClampedToMaxSize(ForceMultiplier);

	//Speed Scaling at Maximum Velocity.
	const float CurrentVelocity = GetVelocity().Length() / 100.f;
	const float SpeedScalingFactor = FMath::Clamp(CurrentVelocity / MaxDesiredVelocity, 0.0f, 1.0f);
	const float ForceScale = FMath::Lerp(2.0f, 1.0f, SpeedScalingFactor);
	
	//Final Force Apply.
	SimSphere->AddForce(LocalForce * ForceScale);
}

void ABall::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookVector.X);
		//AddControllerPitchInput(LookVector.Y);
	}
}

void ABall::Jump(const FInputActionValue& Value)
{
	if(!bIsFalling)
	{
		SimSphere->AddForce(FVector(0.f, 0.f,JumpForce));
	}
}

void ABall::Pause(const FInputActionValue& Value)
{

	if(UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Blue, TEXT("Unpaused!"));
	}
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Blue, TEXT("Paused!"));
	}
}

void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABall::Move);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABall::Look);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ABall::Jump);
		EnhancedInputComponent->BindAction(IA_Pause, ETriggerEvent::Started, this, &ABall::Pause);
	}
}

//Custom Functions
void ABall::SetCamFocus()
{
	Camera->FocusSettings.FocusMethod = ECameraFocusMethod::Manual;
	const float FocusDist = FVector::Dist(Camera->GetComponentLocation(), BallMesh->GetComponentLocation());
	if(UGameplayStatics::IsGamePaused(GetWorld()))
	{
		Camera->SetCurrentAperture(0.1f);
		Camera->FocusSettings.ManualFocusDistance = 10.f;
	}
	else
	{
		Camera->SetCurrentAperture(0.8f);
		Camera->FocusSettings.ManualFocusDistance = FocusDist;
	}
	
}

void ABall::SetCamZoom()
{
	
	const float CurrentVelocity = GetVelocity().Length() / 100.f;
	const float SpeedScalingFactor = FMath::Clamp(CurrentVelocity / MaxDesiredVelocity, 0.0f, 1.0f);
	const float TargetZoom = FMath::Lerp(DefaultZoom,MaxVelocityZoom,  SpeedScalingFactor);

	if(GetWorld())
	{
		const float InterpolatedZoom = FMath::FInterpTo(Camera->CurrentFocalLength, TargetZoom, GetWorld()->DeltaTimeSeconds, CameraZoomInterpSpeed);
		Camera->SetCurrentFocalLength(InterpolatedZoom);
	}
	
}

void ABall::GetTravelledDistance(float Time)
{
	const float CurrentVelocity = GetVelocity().Length() / 100.f;
	DistanceTravelled += CurrentVelocity*Time;
}

void ABall::SetRollAudioIntensity()
{
	const float CurrentVelocity = GetVelocity().Length() / 100.f;
	const float AudioRatio = FMath::Clamp(CurrentVelocity/MaxDesiredVelocity*2.f, 0.f, 1.f);

	if(RollSoundCurve)
	{
		RollAudio->SetVolumeMultiplier(RollSoundCurve->GetFloatValue(AudioRatio));
	}
	
}

void ABall::OnRollAudioFinished()
{
	RollAudio->Play();
}

void ABall::OnBallHit()
{
	
}

bool ABall::FloorTrace(FVector InputLocation)
{

	
	FCollisionQueryParams TraceParams(FName(TEXT("LineTraceByChannel")), true, this);
	const FVector TraceStart = InputLocation;
	FVector TraceEnd = TraceStart + FVector(0.f, 0.f, -25.f);
	
	if(GetWorld())
	{

		FHitResult LineHitResult;
		GetWorld()->LineTraceSingleByChannel(LineHitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

		const float HitDistance = FVector::Dist(LineHitResult.ImpactPoint, InputLocation);

		if(HitDistance>25.f)
		{
			bIsFalling = true;
		}
		else
		{
			bIsFalling = false;
		}
	}
	
	return bIsFalling;
}

