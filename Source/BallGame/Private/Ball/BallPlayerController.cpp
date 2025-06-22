// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/BallPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ball/Ball.h"
#include "Widgets/BallGameHUD.h"

void ABallPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(GameplayMappingContext, 0);
	}

	GetBallPawn();
}

void ABallPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Handle Enhanced Mapping
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABallPlayerController::Move);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABallPlayerController::Look);
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &ABallPlayerController::Zoom);
		EnhancedInputComponent->BindAction(IA_Pause, ETriggerEvent::Started, this, &ABallPlayerController::Paused);
	}
}

ABall* ABallPlayerController::GetBallPawn()
{
	// 1. Check if we already have a valid, cached reference.
	if (CachedBallRef && !CachedBallRef.Get()->IsPendingKillPending())
	{
		return CachedBallRef;
	}

	// 2. If not, try to find it. The Pawn is owned by the PlayerController.
	CachedBallRef = Cast<ABall>(this->GetPawn());
	if (CachedBallRef) return CachedBallRef;

	// 3. If all else fails, return null.
	return nullptr;
}

ABallGameHUD* ABallPlayerController::GetBallGameHUD()
{
	// 1. Check cache first.
	if (CachedHUDRef && !CachedHUDRef.Get()->IsPendingKillPending())
	{
		return CachedHUDRef;
	}

	// 2. If not, find it via the PlayerController.
	CachedHUDRef = Cast<ABallGameHUD>(GetHUD());
	if (CachedHUDRef) return CachedHUDRef;
    
	// 3. Return null if not found.
	return nullptr;
}

void ABallPlayerController::Move(const FInputActionValue& Value)
{
	if(GetBallPawn()) CachedBallRef->Move(Value);
}

void ABallPlayerController::Look(const FInputActionValue& Value)
{
	if(GetBallPawn()) CachedBallRef->Look(Value);
}

void ABallPlayerController::Paused(const FInputActionValue& Value)
{
	if(GetBallGameHUD())
	{
		if(!UGameplayStatics::IsGamePaused(GetWorld()))
		{
			CachedHUDRef->Paused();
		}
		else
		{
			CachedHUDRef->Unpaused();
		}
	}
}

void ABallPlayerController::Zoom(const FInputActionValue& Value)
{
	if (GetBallPawn())
	{
		CachedBallRef->Zoom(Value);
	}
}
