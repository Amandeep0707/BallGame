// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/BGFunctionLibrary.h"
#include "World/BaseUnit.h"

FIntVector UBGFunctionLibrary::WorldToGrid(const FVector& WorldLocation, float GridUnitSize)
{
	return FIntVector(
		FMath::RoundToInt(WorldLocation.X / GridUnitSize),
		FMath::RoundToInt(WorldLocation.Y / GridUnitSize),
		FMath::RoundToInt(WorldLocation.Z / GridUnitSize)
	);
}

FVector UBGFunctionLibrary::GridToWorld(const FIntVector& GridCoord, float GridUnitSize)
{
	return FVector(
		GridCoord.X * GridUnitSize,
		GridCoord.Y * GridUnitSize,
		GridCoord.Z * GridUnitSize
	);
}

FTransform UBGFunctionLibrary::GetAlignedTransform(const FTransform& InTransform, float GridUnitSize)
{
	const FIntVector GridUnitLocation = WorldToGrid(InTransform.GetLocation(), GridUnitSize);
	const FVector AlignedLocation = GridToWorld(GridUnitLocation, GridUnitSize);

	const FQuat AlignedRotation = FRotator(0, FMath::RoundToFloat(InTransform.GetRotation().Rotator().Yaw / 90.f) * 90.f, 0).Quaternion();
	
	return FTransform(AlignedRotation, AlignedLocation);
}

FTransform UBGFunctionLibrary::GetSocketTransform(const ABaseUnit* Unit, const FName& SocketName)
{
	if(!Unit) return FTransform::Identity;
	
	const FTransform ExitSocketTransform = Unit->GetStaticMeshComponent()->GetSocketTransform(SocketName, RTS_World);
	const FVector SocketForwardVector = ExitSocketTransform.GetRotation().GetForwardVector();
	const FVector Location = ExitSocketTransform.GetLocation() + SocketForwardVector * Unit->GetUnitDetails().UnitCellSize / 2.f;
	const FQuat Rotation = ExitSocketTransform.GetRotation();

	return FTransform(Rotation, Location, FVector::OneVector);
}

TArray<FName> UBGFunctionLibrary::GetAllExitSockets(const ABaseUnit* Unit)
{
	TArray<FName> ExitSockets;
	for (const FName& SocketName : Unit->GetStaticMeshComponent()->GetAllSocketNames())
	{
		if (SocketName.ToString().StartsWith(TEXT("Exit_")))
		{
			ExitSockets.Add(SocketName);
		}
	}
	if (ExitSockets.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Current unit '%s' has no exits. Linear path terminated naturally."), *Unit->GetName());
	}
	return ExitSockets;
}

TSubclassOf<ABaseUnit> UBGFunctionLibrary::GetRandomUnit(const TArray<TSubclassOf<ABaseUnit>>& InUnits)
{
	return InUnits[FMath::RandRange(0, InUnits.Num() - 1)];
}