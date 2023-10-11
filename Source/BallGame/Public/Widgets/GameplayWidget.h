// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayWidget.generated.h"

class ABall;
/**
 * 
 */
UCLASS()
class BALLGAME_API UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual bool Initialize() override;	

public:

	//Inherited Parent Functions
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//Custom Variables
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Ball Stats")
	float DistanceTravelled = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Ball Stats")
	float MaxVelocity = 0.f;

protected:

	//Custom Variables
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Ball Stats")
	ABall* BP_Ball;
};
