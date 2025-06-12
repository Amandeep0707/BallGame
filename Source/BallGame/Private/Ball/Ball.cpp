// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/Ball.h"

#include "CineCameraComponent.h"
#include "BallGame/BallGameGameModeBase.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"


ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	SimSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SimSphere);
	SimSphere->SetSimulatePhysics(true);
	SimSphere->SetNotifyRigidBodyCollision(true);
	SimSphere->SetCollisionProfileName(TEXT("BlockAll"));
	SimSphere->SetLinearDamping(0.5f);
	SimSphere->SetAngularDamping(0.5f);
	SimSphere->SetSphereRadius(BallRadius);
	
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
	SimSphere->OnComponentHit.AddDynamic(this, &ABall::OnBallHit);
	
	//Setting Distance travelled to 0 when the game starts
	DistanceTravelled = 0.f;

	//Start Playing Roll Audio
	RollAudio->Play();
	RollAudio->SetVolumeMultiplier(0.f);

	//Initializing Variables
	bIsGameOver = false;
	GameMode = Cast<ABallGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	ChangeMaterial(DefaultMaterial);
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetCamFocus();
	SetCamZoom();
	GetTravelledDistance(DeltaTime);
	SetRollAudioIntensity();

	FloorTrace();
	
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

void ABall::ChangeMaterial(EBallMaterial NewMaterial)
{
	if (MaterialDataAssets.Contains(NewMaterial))
	{
		CurrentMaterial = NewMaterial;
		ApplyMaterialProperties();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to change to an invalid material type."))
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
	if (bIsFalling) return;
	
	const float CurrentVelocity = GetVelocity().Length() / 100.f;
	DistanceTravelled += CurrentVelocity*Time;
}

void ABall::SetRollAudioIntensity()
{
	const float CurrentVelocity = GetVelocity().Length() / 100.f;
	const float AudioRatio = FMath::Clamp(CurrentVelocity/MaxDesiredVelocity*2.f, 0.f, 1.f);

	if(RollSoundCurve && !bIsFalling)
	{
		RollAudio->SetVolumeMultiplier(RollSoundCurve->GetFloatValue(AudioRatio));
	}
	if(bIsFalling)
	{
		RollAudio->Stop();
	}
	
}

void ABall::ApplyMaterialProperties()
{
	const UBallMaterialDataAsset* MaterialData = MaterialDataAssets[CurrentMaterial];
	if (!MaterialData) return;

	const FBallMaterialProperties& Props = MaterialData->Properties;

	// Update Mesh
	BallMesh->SetStaticMesh(Props.BallMesh);
	BallMesh->SetMaterial(0, Props.VisualMaterial);
	
	// Update Physics Properties
	SimSphere->SetMassScale(NAME_None, Props.MassScale);
	SimSphere->SetLinearDamping(Props.LinearDamping);
	SimSphere->SetAngularDamping(Props.AngularDamping);
	SimSphere->SetPhysMaterialOverride(Props.PhysicsMaterial);
	
	// We can also update our gameplay variables
	ForceMultiplier = Props.ForceMultiplier;
}

void ABall::OnRollAudioFinished()
{
	RollAudio->Play();
}

void ABall::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(NormalImpulse.Length()*50.f > HitSoundThreshold)
	{
		HitAudio->Play();
	}
}

bool ABall::FloorTrace()
{
	const FVector TraceStart = SimSphere->GetComponentLocation() + FVector(0.f, 0.f, BallRadius);
	const float TraceDistance = 2.f * BallRadius + 10.f;
	const FVector TraceEnd = TraceStart - FVector(0.f, 0.f, TraceDistance);
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetOwner());
	FHitResult TraceHitResult;
		
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceStart, TraceEnd, BallRadius,
											UEngineTypes::ConvertToTraceType(ECC_Visibility), true, IgnoredActors,
											EDrawDebugTrace::None, TraceHitResult, true, FLinearColor::Green,
											FLinearColor::Red, 1.f);

	const float HitDistance = FVector::Dist(TraceHitResult.ImpactPoint, TraceStart);

	if(HitDistance>TraceDistance)
	{
		bIsFalling = true;
	}
	else
	{
		bIsFalling = false;
	}

	static float FallTimer = 0.f;
	FallTimer++;
	
	if(bIsFalling)
	{
		if(FallTimer >= 120.f && !bIsGameOver)
		{
			bIsGameOver = true;
			if(GameMode)
			{
				GameMode->PlayerFell();
			}
		}
	}

	if(!bIsFalling)
	{
		FallTimer = 0.f;
	}
	
	return bIsFalling;
}