// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Collectible.h"
#include "Ball/Ball.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "World/BallPlayerState.h"

ACollectible::ACollectible()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	CollectibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectibleMesh"));
	CollectibleMesh->SetupAttachment(Root);
	CollectibleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetupAttachment(Root);
	TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));
	TriggerSphere->SetSphereRadius(100.f);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0, 100, 0);
}

void ACollectible::BeginPlay()
{
	Super::BeginPlay();

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ACollectible::OnOverlapBegin);
}

void ACollectible::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABall* Ball = Cast<ABall>(OtherActor))
	{
		APlayerController* PC = Cast<APlayerController>(Ball->GetController());
		if (PC)
		{
			if (ABallPlayerState* PS = PC->GetPlayerState<ABallPlayerState>())
			{
				PS->AddToSessionScore(PointValue);

				// --- FEEDBACK ---
				if (CollectedSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, CollectedSound, GetActorLocation());
				}
				if (CollectedVFX)
				{
					UGameplayStatics::SpawnEmitterAtLocation(this, CollectedVFX, GetActorLocation());
				}

				// The collectible has been collected, so it should be destroyed.
				Destroy();
			}
		}
	}
}

void ACollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

