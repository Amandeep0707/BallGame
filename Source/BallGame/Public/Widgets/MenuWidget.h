// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class ABallGameHUD;
class ABall;

UCLASS()
class BALLGAME_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual bool Initialize() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:

	//Inherited Parent Functions
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//CustomFunctions
	
protected:

	//Custom Variables
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Ball Stats")
	ABall* BP_Ball;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = HUD)
	ABallGameHUD* HUD;
};
