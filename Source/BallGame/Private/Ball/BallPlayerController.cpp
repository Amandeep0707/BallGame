// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/BallPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Ball/Ball.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/BallGameHUD.h"

void ABallPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(GameplayMappingContext, 0);
	}
}

void ABallPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Handle Enhanced Mapping
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABallPlayerController::Move);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABallPlayerController::Look);
		EnhancedInputComponent->BindAction(IA_Pause, ETriggerEvent::Started, this, &ABallPlayerController::Paused);
	}
}

void ABallPlayerController::Move(const FInputActionValue& Value)
{
	ABall* Ball = Cast<ABall>(GetPawn());
	if(Ball)
	{
		Ball->Move(Value);
	}
}

void ABallPlayerController::Look(const FInputActionValue& Value)
{
	ABall* Ball = Cast<ABall>(GetPawn());
	if(Ball)
	{
		Ball->Look(Value);
	}
}

void ABallPlayerController::Paused(const FInputActionValue& Value)
{
	ABall* Ball = Cast<ABall>(GetPawn());
	if(Ball)
	{
		ABallGameHUD* HUD = Cast<ABallGameHUD>(GetHUD());
		if(HUD)
		{
			if(!UGameplayStatics::IsGamePaused(GetWorld()))
			{
				HUD->Paused();
			}
			else
			{
				HUD->Unpaused();
			}
		}
	}
}