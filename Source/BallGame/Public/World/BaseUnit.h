// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "BaseUnit.generated.h"

USTRUCT(BlueprintType)
struct FUnitDetails
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Structure")
	float UnitCellSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Structure")
	TArray<FIntVector> OccupiedCellsOffsets;

	FUnitDetails()
	{
		UnitCellSize = 400.f;
		OccupiedCellsOffsets.Add(FIntVector::ZeroValue);
	}
};

UCLASS()
class BALLGAME_API ABaseUnit : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ABaseUnit();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable, Category = "Unit")
	FORCEINLINE FUnitDetails GetUnitDetails() const { return UnitDetails; }

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Details")
	FUnitDetails UnitDetails;

	// --- DEBUG ---
	UFUNCTION(CallInEditor, BlueprintCallable, Category="DEBUG")
	void DebugCells();
};

inline void ABaseUnit::Destroyed()
{
	Super::Destroyed();
	FlushPersistentDebugLines(GetWorld());
}
