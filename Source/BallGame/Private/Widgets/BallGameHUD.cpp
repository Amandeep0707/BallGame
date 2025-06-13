// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BallGameHUD.h"
#include "Ball/Ball.h"
#include "Ball/BallPlayerController.h"
#include "BallGame/BallGameGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/GameplayWidget.h"
#include "Widgets/MenuWidget.h"


ABallGameHUD::ABallGameHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABallGameHUD::BeginPlay()
{
	Super::BeginPlay();

	//Enable GameplayHUD Widget
	if(GameplayHUD)
	{
		HUD = CreateWidget<UGameplayWidget>(GetWorld(), GameplayHUD);
		HUD->SetHUDRef(this);
		HUD->AddToViewport();
	}

	//Enable MenuHUD Widget
	if(MenuHUD)
	{
		Menu = CreateWidget<UMenuWidget>(GetWorld(), MenuHUD);
		Menu->AddToViewport();
	}

	if(GetGameMode()) PlayerLivesUpdate.Broadcast(CachedGameModeRef->GetCurrentPlayerLives());
}

void ABallGameHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Custom Functions
void ABallGameHUD::Paused()
{
	
	if(Menu)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		Menu->SetVisibility(ESlateVisibility::Visible);
		if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(Menu->TakeWidget());
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(true);
		}
	}
}

void ABallGameHUD::Unpaused()
{
	
	if(Menu)
	{
		if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			const FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(false);
		}
		Menu->SetVisibility(ESlateVisibility::Hidden);
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

ABall* ABallGameHUD::GetBallPawn()
{
	// 1. Check if we already have a valid, cached reference.
	if (CachedBallRef && !CachedBallRef.Get()->IsPendingKillPending())
	{
		return CachedBallRef;
	}

	// 2. If not, try to find it. The Pawn is owned by the PlayerController.
	if (const ABallPlayerController* PC = Cast<ABallPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		CachedBallRef = Cast<ABall>(PC->GetPawn());
		return CachedBallRef;
	}

	// 3. If all else fails, return null.
	return nullptr;
}

ABallGameGameModeBase* ABallGameHUD::GetGameMode()
{
	// 1. Check cache first.
	if (CachedGameModeRef && !CachedGameModeRef.Get()->IsPendingKillPending())
	{
		return CachedGameModeRef;
	}

	// 2. If not, find it via UGameplayStatistics
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld()))
	{
		CachedGameModeRef = Cast<ABallGameGameModeBase>(GameMode);
		return CachedGameModeRef;
	}
    
	// 3. Return null if not found.
	return nullptr;
}
