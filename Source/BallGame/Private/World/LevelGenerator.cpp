// Fill out your copyright notice in the Description page of Project Settings.


#include "World/LevelGenerator.h"
#include "World/BaseUnit.h"
#include "Utility/BGFunctionLibrary.h"

ALevelGenerator::ALevelGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelGenerator::GenerateLinearMap()
{
	ClearMap();

	// Gather blueprints for continuing the path.
	TArray<TSubclassOf<ABaseUnit>> ContinuationUnits;
	ContinuationUnits.Append(StraightUnits);
	ContinuationUnits.Append(TurnUnits);
	ContinuationUnits.Append(StairUnits);
	ContinuationUnits.Append(JunctionUnits);

	if (ContinuationUnits.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No 'NonEndCap' Blueprints assigned. Cannot generate linear map."));
		return;
	}

	// --- SPAWN THE INITIAL UNIT ---
	const FTransform InitialSpawnTransform = {FQuat::Identity, FVector::ZeroVector};
	ABaseUnit* CurrentUnit = TrySpawnUnit(UBGFunctionLibrary::GetRandomUnit(StraightUnits), InitialSpawnTransform);
	if (!CurrentUnit)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn initial unit at origin."));
		return;
	}
	OpenSockets.Emplace(CurrentUnit, FName("Entry_01"));
	int32 UnitsSpawned = 1;

	// --- MAIN GENERATION LOOP ---
	while (UnitsSpawned < NumberOfUnits)
	{
		// Find all exit sockets on the current piece. If not break the loop.
		TArray<FName> ExitSocketsOnCurrent = UBGFunctionLibrary::GetAllExitSockets(CurrentUnit);
		if (ExitSocketsOnCurrent.IsEmpty()) break;
		
		// Pick one random socket to continue as main path
		const int32 MainPathIndex = FMath::RandRange(0, ExitSocketsOnCurrent.Num() - 1);
		FName MainPathSocketName = ExitSocketsOnCurrent[MainPathIndex];
		
		// Remove the main path socket from the list. All remaining sockets are now side-branches.
		ExitSocketsOnCurrent.RemoveAt(MainPathIndex);

		// Add all side-branch sockets to our master `OpenSockets` list for later capping.
		for (const FName& SideSocketName : ExitSocketsOnCurrent)
		{
			OpenSockets.Emplace(CurrentUnit, SideSocketName);
		}

		// --- ATTEMPT TO SPAWN ALONG THE MAIN PATH ---
		FTransform SpawnTransform = UBGFunctionLibrary::GetSocketTransform(CurrentUnit, MainPathSocketName);
		
		ABaseUnit* NextUnitInChain = nullptr;
		constexpr int32 MaxFindAttempts = 10;
		for (int32 Attempt = 0; Attempt < MaxFindAttempts; ++Attempt)
		{
			const TSubclassOf<ABaseUnit> CandidateUnitClass = UBGFunctionLibrary::GetRandomUnit(ContinuationUnits);
			if (IsUnitPlacementViable(CandidateUnitClass, SpawnTransform))
			{
				NextUnitInChain = TrySpawnUnit(CandidateUnitClass, SpawnTransform);
				if (NextUnitInChain) break;
			}
		}

		if (NextUnitInChain)
		{
			CurrentUnit = NextUnitInChain;
			UnitsSpawned++;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Linear path blocked at unit '%s'. Ending growth phase."), *CurrentUnit->GetName());
			OpenSockets.Emplace(CurrentUnit, MainPathSocketName);
			break; 
		}
	}
	
	// Adding Exit sockets of the final units to "OpenSockets" list.
	if (CurrentUnit) {
		for (const FName& SocketName : UBGFunctionLibrary::GetAllExitSockets(CurrentUnit)) OpenSockets.Emplace(CurrentUnit, SocketName);
	}

	// --- TERMINATION LOOP ---
	if (OpenSockets.Num() > 0 && EndCapUnits.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Linear growth phase complete. Now sealing %d total open sockets."), OpenSockets.Num());
	}
	
	while (OpenSockets.Num() > 0)
	{
		FOpenSocket SocketToCap = OpenSockets.Pop();
		if (!SocketToCap.Unit) continue;

		if(TrySpawnUnit(UBGFunctionLibrary::GetRandomUnit(EndCapUnits), UBGFunctionLibrary::GetSocketTransform(SocketToCap.Unit, SocketToCap.SocketName)))
		{
			UnitsSpawned++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Linear generation finished. A total of %d units were placed."), UnitsSpawned);
}

void ALevelGenerator::GenerateBranchedMap()
{
	ClearMap();

	// Create a list of all blueprints except "End" Unit
	TArray<TSubclassOf<ABaseUnit>> ContinuationUnits;
	ContinuationUnits.Append(StraightUnits);
	ContinuationUnits.Append(TurnUnits);
	ContinuationUnits.Append(StairUnits);
	ContinuationUnits.Append(JunctionUnits);

	if (ContinuationUnits.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No 'NonEndCap' Blueprints assigned. Only the first piece can be spawned."));
	}
	if (EndCapUnits.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No 'EndCap' Blueprints assigned. Dead ends will not be sealed."));
	}
	
	// --- SPAWN THE INITIAL UNIT ---
	TSubclassOf<ABaseUnit> InitialUnitClass = UBGFunctionLibrary::GetRandomUnit(StraightUnits);

	if (!InitialUnitClass) {
		UE_LOG(LogTemp, Error, TEXT("No valid initial blueprint class found."));
		return;
	}

	FTransform InitialSpawnTransform = {GetActorRotation(), GetActorLocation(), FVector::OneVector};
	ABaseUnit* FirstUnit = TrySpawnUnit(InitialUnitClass, InitialSpawnTransform);
	if (!FirstUnit)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn initial unit. Generation cannot start."));
		return;
	}
	OpenSockets.Emplace(FirstUnit, FName("Entry_01"));

	for (const FName& SocketName : FirstUnit->GetStaticMeshComponent()->GetAllSocketNames())
	{
		if (SocketName.ToString().StartsWith(TEXT("Exit_")))
		{
			OpenSockets.Emplace(FirstUnit, SocketName);
		}
	}

	int32 UnitsSpawned = 1;
	int32 TotalAttempts = 0;

	// --- MAIN GENERATION LOOP ---
	while (UnitsSpawned < NumberOfUnits && OpenSockets.Num() > 0 && TotalAttempts < NumberOfUnits * 20)
	{
		TotalAttempts++;
		
		const int32 SocketIndex = FMath::RandRange(0, OpenSockets.Num() - 1);
		FOpenSocket ChosenExitSocket = OpenSockets[SocketIndex];
		OpenSockets.RemoveAt(SocketIndex);
		
		if (!ChosenExitSocket.Unit) continue;

		// Get Spawn Transform
		FTransform SpawnTransform = UBGFunctionLibrary::GetSocketTransform(ChosenExitSocket.Unit, ChosenExitSocket.SocketName);
		
		ABaseUnit* NewUnit = nullptr;
		constexpr int32 MaxFindAttempts = 10;
		
		for (int32 Attempt = 0; Attempt < MaxFindAttempts; ++Attempt)
		{
			if (ContinuationUnits.IsEmpty()) break;
			const TSubclassOf<ABaseUnit> CandidateUnitClass = UBGFunctionLibrary::GetRandomUnit(ContinuationUnits);
			if (IsUnitPlacementViable(CandidateUnitClass, SpawnTransform))
			{
				NewUnit = TrySpawnUnit(CandidateUnitClass, SpawnTransform);
				if (NewUnit) break;
			}
		}
		if (!NewUnit && EndCapUnits.Num() > 0)
		{
			const TSubclassOf<ABaseUnit> EndCapClass = UBGFunctionLibrary::GetRandomUnit(EndCapUnits);
			NewUnit = TrySpawnUnit(EndCapClass, SpawnTransform);
		}
		if (NewUnit)
		{
			UnitsSpawned++;
			for (const FName& NewSocketName : NewUnit->GetStaticMeshComponent()->GetAllSocketNames())
			{
				if (NewSocketName.ToString().StartsWith(TEXT("Exit_")))
				{
					OpenSockets.Emplace(NewUnit, NewSocketName);
				}
			}
		}
	}
	// --- MAIN LOOP END ---
	

	// --- TERMINATION LOOP ---
	if (OpenSockets.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Growth phase complete. Now sealing %d remaining open sockets."), OpenSockets.Num());
	}
	
	while (OpenSockets.Num() > 0)
	{
		// Get the last open socket and remove it from the list.
		FOpenSocket SocketToCap = OpenSockets.Pop();

		if (!SocketToCap.Unit) continue;

		// We must have EndCapUnits to seal the openings.
		if (EndCapUnits.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("No EndCap units available to seal open socket. Stopping termination phase."));
			break;
		}

		// Get the transform for placing the end cap.
		FTransform SpawnTransform = UBGFunctionLibrary::GetSocketTransform(SocketToCap.Unit, SocketToCap.SocketName);

		// Pick a random end cap and try to spawn it.
		const TSubclassOf<ABaseUnit> EndCapClass = UBGFunctionLibrary::GetRandomUnit(EndCapUnits);
		ABaseUnit* NewUnit = TrySpawnUnit(EndCapClass, SpawnTransform);
		
		if (NewUnit)
		{
			UnitsSpawned++;
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Generation finished. A total of %d units were placed."), UnitsSpawned);
}

void ALevelGenerator::ClearMap()
{
	FlushPersistentDebugLines(GetWorld());
	for (const FUnitData& Data : SpawnedUnitsData)
	{
		if (Data.Unit)
		{
			Data.Unit->Destroy();
		}
	}
	SpawnedUnitsData.Empty();
	AllOccupiedCells.Empty();
	LastSpawnedUnit = nullptr;
	OpenSockets.Empty();
}

void ALevelGenerator::DebugAllOccupiedCells(const TSet<FIntVector> &Cells, const float GridUnitSize) const
{
	
	FlushPersistentDebugLines(GetWorld());
	if (Cells.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("No cells are occupied."));
		return;
	}

	for (const FIntVector& CellCoord : Cells)
	{
		FVector WorldCenter = UBGFunctionLibrary::GridToWorld(CellCoord, GridUnitSize);
		DrawDebugBox(
			GetWorld(),
			WorldCenter + FVector(0.f, 0.f, GridUnitSize / 2.f),
			FVector(GridUnitSize / 2.f * 0.9f),
			FColor::Green,
			true,
			-1.f,
			0,
			5.f
		);
	}
}

ABaseUnit* ALevelGenerator::TrySpawnUnit(TSubclassOf<ABaseUnit> UnitClass, const FTransform& Transform)
{
    if (!UnitClass) return nullptr;

	// Create default object from the given class
    const ABaseUnit* Unit = UnitClass->GetDefaultObject<ABaseUnit>();

	// Calculate Aligned Location
    FIntVector GridUnitLocation = UBGFunctionLibrary::WorldToGrid(Transform.GetLocation(), Unit->GetUnitDetails().UnitCellSize);
	const FTransform AlignedTransform = UBGFunctionLibrary::GetAlignedTransform(Transform, Unit->GetUnitDetails().UnitCellSize);

    // --- COLLISION CHECK ---
    TArray<FIntVector> TestCells;
    for (const FIntVector& CellOffset : Unit->GetUnitDetails().OccupiedCellsOffsets)
    {
        FVector FloatOffset = FVector(CellOffset);
        FVector RotatedFloatOffset = AlignedTransform.GetRotation().RotateVector(FloatOffset);

        FIntVector RotatedGridOffset = FIntVector(
            FMath::RoundToInt(RotatedFloatOffset.X),
            FMath::RoundToInt(RotatedFloatOffset.Y),
            FMath::RoundToInt(RotatedFloatOffset.Z)
        );

        FIntVector WorldCell = GridUnitLocation + RotatedGridOffset;

        if (AllOccupiedCells.Contains(WorldCell)) return nullptr;
        TestCells.Add(WorldCell);
    }

    // --- SPAWNING ACTOR ---
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    ABaseUnit* NewUnit = GetWorld()->SpawnActor<ABaseUnit>(UnitClass, AlignedTransform.GetLocation(), AlignedTransform.GetRotation().Rotator(), SpawnParams);

    if (NewUnit)
    {
        UE_LOG(LogTemp, Log, TEXT("Spawned Unit at %s, occupying %d cells."), *AlignedTransform.GetLocation().ToString(), TestCells.Num());

        AllOccupiedCells.Append(TestCells);

    	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
    	NewUnit->AttachToActor(this, AttachmentRules);
        FUnitData NewUnitData;
        NewUnitData.Unit = NewUnit;
        NewUnitData.OccupiedWorldCells = TestCells;
        SpawnedUnitsData.Add(NewUnitData);
    }

	FlushPersistentDebugLines(GetWorld());
    return NewUnit;
}

// --- HELPER FUNCTIONS ---
// To be transferred into a function library

bool ALevelGenerator::IsUnitPlacementViable(const TSubclassOf<ABaseUnit>& UnitClass, const FTransform& UnitTransform) const
{
	if (!UnitClass) return false;

	const ABaseUnit* DefaultUnit = UnitClass->GetDefaultObject<ABaseUnit>();
	if (!DefaultUnit) return false;

	// Now, iterate through all of this candidate unit's potential EXIT sockets.
	for (const FName& SocketName : DefaultUnit->GetStaticMeshComponent()->GetAllSocketNames())
	{
		if (SocketName.ToString().StartsWith(TEXT("Exit_")))
		{
			// Get the future WORLD transform of this exit socket if the unit were placed.
			FTransform ExitSocketLocalTransform = DefaultUnit->GetStaticMeshComponent()->GetSocketTransform(SocketName, RTS_World);
			FTransform ExitSocketWorldTransform = ExitSocketLocalTransform * UBGFunctionLibrary::GetAlignedTransform(UnitTransform, DefaultUnit->GetUnitDetails().UnitCellSize);

			// Calculate the grid cell location ONE STEP IN FRONT of this future exit.
			FVector LookAheadLocation = ExitSocketWorldTransform.GetLocation() + ExitSocketWorldTransform.GetRotation().GetForwardVector() * DefaultUnit->GetUnitDetails().UnitCellSize / 2.f;
			FIntVector LookAheadGridCell = UBGFunctionLibrary::WorldToGrid(LookAheadLocation, DefaultUnit->GetUnitDetails().UnitCellSize);

			// Check if this "look-ahead" cell is already occupied by a previously placed unit.
			if (AllOccupiedCells.Contains(LookAheadGridCell))
			{
				// This exit is blocked! The entire placement is not viable.
				return false;
			}
		}
	}

	// All exits are clear. The placement is viable.
	return true;
}

