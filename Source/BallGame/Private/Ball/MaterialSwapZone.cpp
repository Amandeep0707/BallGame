// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/MaterialSwapZone.h"
#include "Ball/Ball.h"
#include "Ball/BallTypes.h"
#include "Components/BoxComponent.h"


AMaterialSwapZone::AMaterialSwapZone()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(Root);

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(Root);
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
	
	TargetMaterial = EBallMaterial::Wood;
}

void AMaterialSwapZone::BeginPlay()
{
	Super::BeginPlay();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AMaterialSwapZone::OnOverlapBegin);
}

void AMaterialSwapZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABall* Ball = Cast<ABall>(OtherActor))
	{
		Ball->ChangeMaterial(TargetMaterial);
		
		DrawDebugString(GetWorld(), GetActorLocation(),
		                FString::Printf(TEXT("Changed Material to %s"), *UEnum::GetValueAsString(TargetMaterial)), nullptr,
		                FColor::Emerald, 10.f);
	}
}

void AMaterialSwapZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

