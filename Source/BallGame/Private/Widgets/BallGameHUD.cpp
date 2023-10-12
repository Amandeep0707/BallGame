// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BallGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/GameplayWidget.h"


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
	
}

void ABallGameHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

