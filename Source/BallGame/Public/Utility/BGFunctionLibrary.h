// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BGFunctionLibrary.generated.h"

UCLASS()
class BALLGAME_API UBGFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*
	 * --- Grid Generator Utility Functions ---
	 */

	/** @brief Converts a world location FVector to an integer grid coordinate. */
	UFUNCTION(BlueprintCallable, Category="Unit Generation")
	static FIntVector WorldToGrid(const FVector& WorldLocation, float GridUnitSize);

	/** @brief Converts an integer grid coordinate back to a world location FVector. */
	UFUNCTION(BlueprintCallable, Category="Unit Generation")
	static FVector GridToWorld(const FIntVector& GridCoord, float GridUnitSize);

	/**
	 * @brief Returns the Grid Aligned Transform for a given transform
	 * @param InTransform Input Transform
	 * @param GridUnitSize Grid Size
	 */
	UFUNCTION(BlueprintCallable, Category="Unit Generation")
	static FTransform GetAlignedTransform(const FTransform &InTransform, float GridUnitSize);

	/**
	 * @brief This function returns transform of a given socket in a grit unit space
	 * @param Unit Unit to find the socket name
	 * @param SocketName Socket name to find the transform for
	 * @return Returns a transform for the given socket
	 */
	UFUNCTION(BlueprintCallable, Category="Unit Generation")
	static FTransform GetSocketTransform(const ABaseUnit* Unit, const FName &SocketName);

	/**
	 * Get All Exit Sockets present in the unit
	 * @param Unit Unit to get the Sockets
	 * @return Exit Sockets
	 */
	UFUNCTION(BlueprintCallable, Category="Unit Generation")
	static TArray<FName> GetAllExitSockets(const ABaseUnit* Unit);

	/**
	 * @brief Get a Random Unit from the Array
	 * @param InUnits Input Array
	 * @return RandomUnit
	 */
	UFUNCTION(BlueprintCallable, Category="Unit Generation")
	static TSubclassOf<ABaseUnit> GetRandomUnit(const TArray<TSubclassOf<ABaseUnit>>& InUnits);
};
