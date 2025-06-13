// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Checkpoint.h"
#include "Components/BoxComponent.h"
#include "Ball/Ball.h"
#include "Kismet/GameplayStatics.h"
#include "BallGame/BallGameGameModeBase.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(Root);
	
	SnapPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SnapPoint"));
	SnapPoint->SetupAttachment(Root);

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(Root);
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);
}

void ACheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABall* Ball = Cast<ABall>(OtherActor))
	{
		// Don't re-trigger if the ball is already being controlled
		if (Ball->IsAutoPiloting()) return;

		if (ABallGameGameModeBase* GameMode = Cast<ABallGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// Tell the GameMode to update the last checkpoint transform
			GameMode->UpdateCheckpoint(SnapPoint->GetComponentLocation());

			// Tell the ball to move to our snap point
			Ball->StartAutoPilot(SnapPoint->GetComponentLocation(), this);

			// Deactivate this trigger to prevent it from firing again immediately
			TriggerVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Checkpoint Saved!!!"), nullptr,
						FColor::Yellow, 2.f);
		}
	}
}

void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

