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

	// --- UI Refs ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> GameplayHUD;

	UPROPERTY(VisibleDefaultsOnly, Category = "Widgets")
	UGameplayWidget* HUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> MenuHUD;

	UPROPERTY(VisibleDefaultsOnly, Category = "Widgets")
	UMenuWidget* Menu;

	// ---  Cached Refs ---
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	TObjectPtr<ABall> CachedBallRef;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	TObjectPtr<ABallGameGameModeBase> CachedGameModeRef;

private:

	UFUNCTION(BlueprintCallable, Category = "Game Mode|References")
	ABall* GetBallPawn();

	UFUNCTION(BlueprintCallable, Category = "Game Mode|References")
	ABallGameGameModeBase* GetGameMode();
};