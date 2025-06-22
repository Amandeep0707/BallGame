// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/NonPlayableBall.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"


ANonPlayableBall::ANonPlayableBall(): DefaultMaterial(EBallMaterial::Wood)
{
	PrimaryActorTick.bCanEverTick = false;

	SimSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SimSphere);
	SimSphere->SetSimulatePhysics(true);
	SimSphere->SetNotifyRigidBodyCollision(true);
	SimSphere->SetCollisionProfileName(TEXT("BlockAll"));
	SimSphere->SetLinearDamping(0.5f);
	SimSphere->SetAngularDamping(0.5f);
	SimSphere->SetSphereRadius(BallRadius);

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
	BallMesh->SetupAttachment(SimSphere);

	ChangeMaterial(DefaultMaterial);

}

void ANonPlayableBall::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TraceTimer, this, &ThisClass::CheckFalling, 5.f, true, 5.f);
}

void ANonPlayableBall::ChangeMaterial(EBallMaterial NewMaterial)
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

void ANonPlayableBall::ApplyMaterialProperties()
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
}

void ANonPlayableBall::CheckFalling()
{
	const FVector TraceStart = SimSphere->GetComponentLocation();
	const float TraceDistance = 2.f * BallRadius;
	const FVector TraceEnd = TraceStart - FVector(0.f, 0.f, TraceDistance);
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	FHitResult TraceHitResult;
		
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceStart, TraceEnd, BallRadius,
											UEngineTypes::ConvertToTraceType(ECC_Visibility), true, IgnoredActors,
											EDrawDebugTrace::ForDuration, TraceHitResult, true, FLinearColor::Green,
											FLinearColor::Red, 1.f);
	
	// const float HitDistance = FVector::Dist(TraceHitResult.ImpactPoint, TraceStart);
	
	if(!bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Called Destroy"));
		GetWorld()->GetTimerManager().ClearTimer(TraceTimer);
		SetLifeSpan(1.f);
	}
}
