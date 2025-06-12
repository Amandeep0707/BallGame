// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BallGameHUD.h"
#include "Ball/Ball.h"
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

	// Save Ball Ref
	BP_Ball = Cast<ABall>(GetOwningPawn());

	// Save GameMode Ref
	GameMode = Cast<ABallGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
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