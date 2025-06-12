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
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
		if (ABallGameGameModeBase* GameMode = Cast<ABallGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// Tell the GameMode to update the last checkpoint
			GameMode->UpdateCheckpoint(GetActorTransform());
            
			// Optional: Deactivate this checkpoint after use
			TriggerVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			// You could also play a sound or visual effect here

			DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Checkpoint Saved!!!"), nullptr,
						FColor::Yellow, 10.f);
		}
	}
}

void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

