// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameplayWidget.h"
#include "Ball/Ball.h"
#include "World/BallPlayerState.h"
#include "Components/TextBlock.h"

bool UGameplayWidget::Initialize()
{
	Super::Initialize();
	return true;
}

void UGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(ABall* Ball = Cast<ABall>(GetOwningPlayerPawn()))
	{
		BP_Ball = Ball;
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}

	// Get the PlayerState and bind our function to its delegate.
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		if (ABallPlayerState* PS = PC->GetPlayerState<ABallPlayerState>())
		{
			// Bind our local function to the delegate in the PlayerState.
			PS->OnScoreChanged.AddDynamic(this, &UGameplayWidget::OnScoreChanged);

			// Set the initial score text when the widget is first created.
			OnScoreChanged(PS->GetScore());
		}
	}
	
}

void UGameplayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	MaxVelocity = BP_Ball->GetMaxDesiredVelocity();
}

void UGameplayWidget::OnScoreChanged(int32 NewScore)
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(NewScore));
	}
}
