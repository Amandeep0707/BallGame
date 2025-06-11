// Fill out your copyright notice in the Description page of Project Settings.


#include "World/BaseUnit.h"


ABaseUnit::ABaseUnit()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseUnit::DebugCells()
{
	FlushPersistentDebugLines(GetWorld());
	for (const FIntVector &Cell : UnitDetails.OccupiedCellsOffsets)
	{
		DrawDebugBox(
			GetWorld(),
			GetActorLocation() + FVector(0.f, 0.f, UnitDetails.UnitCellSize / 2.f) + FVector(Cell.X * UnitDetails.UnitCellSize, Cell.Y * UnitDetails.UnitCellSize, Cell.Z * UnitDetails.UnitCellSize),
			FVector(UnitDetails.UnitCellSize / 2.f),
			FColor::Blue,
			true,
			-1.f,
			0,
			3.f
			);
	}

	if (GetStaticMeshComponent()->GetAllSocketNames().Num() > 0)
	{
		for (const FName Socket : GetStaticMeshComponent()->GetAllSocketNames())
		{
			FTransform SocketTransform = GetStaticMeshComponent()->GetSocketTransform(Socket, RTS_World);
			DrawDebugDirectionalArrow(
				GetWorld(),
				SocketTransform.GetLocation(),
				SocketTransform.GetLocation() + (SocketTransform.GetRotation().GetForwardVector() * FVector(30.f)),
				15.f,
				Socket.ToString().StartsWith(TEXT("Entry_")) ? FColor::Green : FColor::Red,
				true,
				-1.f,
				100,
				1.5f
			);
		}
	}
}
