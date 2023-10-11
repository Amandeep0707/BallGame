// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameplayWidget.h"
#include "Ball/Ball.h"

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
}

void UGameplayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	DistanceTravelled = BP_Ball->DistanceTravelled;
	MaxVelocity = BP_Ball->MaxDesiredVelocity;
}
