// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BallGameHUD.generated.h"

class UGameplayWidget;

UCLASS()
class BALLGAME_API ABallGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	ABallGameHUD();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Ball Parameters")
	TSubclassOf<UUserWidget> GameplayHUD;

	UGameplayWidget* HUD;

};