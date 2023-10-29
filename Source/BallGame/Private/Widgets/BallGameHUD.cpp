// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BallGameHUD.h"

#include "Ball/Ball.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/GameplayWidget.h"
#include "Widgets/MenuWidget.h"
#include "Ball/Ball.h"


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
		HUD->AddToViewport();
	}

	//Enable MenuHUD Widget
	if(MenuHUD)
	{
		Menu = CreateWidget<UMenuWidget>(GetWorld(), MenuHUD);
		Menu->AddToViewport();
	}

	//Cast to BP_Ball
	BP_Ball = Cast<ABall>(GetOwningPawn());
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