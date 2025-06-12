// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BallGameHUD.generated.h"

class ABall;
class ABallGameGameModeBase;
class UGameplayWidget;
class UMenuWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLivesUpdate, int32, RemainingLives);

UCLASS()
class BALLGAME_API ABallGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	ABallGameHUD();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//CustomFunctions
	void Paused();
	void Unpaused();

	UPROPERTY(BlueprintAssignable, Category="Gameplay")
	FOnPlayerLivesUpdate PlayerLivesUpdate;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> GameplayHUD;

	UPROPERTY(VisibleDefaultsOnly, Category = "Widgets")
	UGameplayWidget* HUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> MenuHUD;

	UPROPERTY(VisibleDefaultsOnly, Category = "Widgets")
	UMenuWidget* Menu;

	UPROPERTY(VisibleDefaultsOnly, Category = "Ball Stats")
	ABall* BP_Ball;

	UPROPERTY(VisibleDefaultsOnly, Category = "GameData")
	ABallGameGameModeBase* GameMode;
};