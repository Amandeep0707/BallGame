// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "BallPlayerController.generated.h"

class ABallGameHUD;
class UInputMappingContext;
class UInputAction;

UCLASS()
class BALLGAME_API ABallPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	virtual void SetupInputComponent() override;

protected:
	
	virtual void BeginPlay() override;

	//Input Parameters
	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputMappingContext* GameplayMappingContext;

	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputMappingContext* MenuMappingContext;

	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, Category = Inputs)
	UInputAction* IA_Pause;
	
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);
	
	void Paused(const FInputActionValue& Value);

private:
	
};
