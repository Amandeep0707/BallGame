// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

class ABaseUnit;

USTRUCT(BlueprintType)
struct FUnitData
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Structure")
	ABaseUnit* Unit;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Structure")
	TArray<FIntVector> OccupiedWorldCells;

	FUnitData()
	{
		Unit = nullptr;
		OccupiedWorldCells = TArray<FIntVector>();
	}
};

USTRUCT(BlueprintType)
struct FOpenSocket
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Structure")
	ABaseUnit* Unit;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Structure")
	FName SocketName;

	FOpenSocket()
	{
		Unit = nullptr;
		SocketName = FName("");
	}
	
	FOpenSocket(ABaseUnit* InUnit, FName InSocketName) 
	{
		Unit = InUnit;
		SocketName = InSocketName;
	}
};

UCLASS()
class BALLGAME_API ALevelGenerator : public AActor
{
	GENERATED_BODY()

public:
	ALevelGenerator();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Unit Generation")
	void GenerateLinearMap();
	
	UFUNCTION(BlueprintCallable, Category = "Unit Generation")
	void GenerateBranchedMap();

	UFUNCTION(BlueprintCallable, Category="Unit Generation")
	void DebugAllOccupiedCells(const TSet<FIntVector> &Cells, const float GridUnitSize) const;

protected:
	virtual void BeginPlay() override;

private:

	// --- CONFIGURATION ---
	UPROPERTY(EditAnywhere, Category="Unit Generation | Defaults", meta=(AllowPrivateAccess = "true"))
	int32 NumberOfUnits = 10;

	// --- CATEGORIZED BLUEPRINTS ---
	UPROPERTY(EditDefaultsOnly, Category="Unit Generation | Blueprints", meta=(AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ABaseUnit>> StraightUnits;
	
	UPROPERTY(EditDefaultsOnly, Category="Unit Generation | Blueprints", meta=(AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ABaseUnit>> TurnUnits;

	UPROPERTY(EditDefaultsOnly, Category="Unit Generation | Blueprints", meta=(AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ABaseUnit>> StairUnits;

	UPROPERTY(EditDefaultsOnly, Category="Unit Generation | Blueprints", meta=(AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ABaseUnit>> JunctionUnits;

	UPROPERTY(EditDefaultsOnly, Category="Unit Generation | Blueprints", meta=(AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ABaseUnit>> EndCapUnits;
	
	// --- STATE TRACKING ---
	UPROPERTY(BlueprintReadOnly, Category="Unit Generation | State", meta=(AllowPrivateAccess = "true"))
	TArray<FUnitData> SpawnedUnitsData;

	UPROPERTY(BlueprintReadOnly, Category="Unit Generation | State", meta=(AllowPrivateAccess = "true"))
	TSet<FIntVector> AllOccupiedCells;

	UPROPERTY(BlueprintReadOnly, Category="Unit Generation | State", meta=(AllowPrivateAccess = "true"))
	TArray<FOpenSocket> OpenSockets;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Unit Generation | State", meta=(AllowPrivateAccess = "true"))
	ABaseUnit* LastSpawnedUnit = nullptr;

	
	/**
	 * @brief Tries to spawn a specific unit blueprint at a given transform.
	 * @brief Returns the spawned actor if successful, nullptr otherwise.
	 */
	ABaseUnit* TrySpawnUnit(TSubclassOf<ABaseUnit> UnitClass, const FTransform& Transform);
	
	/**
	 * @brief Checks if a unit can be placed without its exits being immediately blocked.
	 * @brief This is a "look-ahead" check performed BEFORE spawning.
	 * @param UnitClass Unit class to check for
	 * @param UnitTransform Unit class transform
	 * @return True if the placement is viable, false otherwise.
	 */
	bool IsUnitPlacementViable(const TSubclassOf<ABaseUnit>& UnitClass, const FTransform& UnitTransform) const;

	UFUNCTION(BlueprintCallable, Category = "Unit Generation")
	void ClearMap();
};
