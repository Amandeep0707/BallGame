// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MenuWidget.h"
#include "Ball/Ball.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/BallGameHUD.h"

bool UMenuWidget::Initialize()
{
	Super::Initialize();
	
	return true;
}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Cast to BP_Ball
	if(ABall* Ball = Cast<ABall>(GetOwningPlayerPawn()))
	{
		BP_Ball = Ball;
	}

	SetVisibility(ESlateVisibility::Hidden);

	//Initialize HUD Variable
	if(const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		HUD = Cast<ABallGameHUD>(PlayerController->GetHUD());
	}
}

void UMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FReply UMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey InputKey = InKeyEvent.GetKey();
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
