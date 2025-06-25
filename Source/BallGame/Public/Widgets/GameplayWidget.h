// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameplayWidget.generated.h"

class ABall;
class ABallGameHUD;

UCLASS()
class BALLGAME_API UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual bool Initialize() override;

public:

	// --- Setter Function ---
	FORCEINLINE void SetHUDRef(ABallGameHUD* Ref) {GameHUD = Ref; };

	//Inherited Parent Functions
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Ball Stats")
	float MaxVelocity = 0.f;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

	UFUNCTION()
	void OnScoreChanged(int32 NewScore);

protected:

	//Custom Variables
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Ball Stats")
	ABall* BP_Ball;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Ball Stats")
	ABallGameHUD* GameHUD;
};
