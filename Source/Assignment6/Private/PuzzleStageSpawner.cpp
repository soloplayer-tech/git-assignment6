#include "PuzzleStageSpawner.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

APuzzleStageSpawner::APuzzleStageSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APuzzleStageSpawner::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World) return;

	for (int32 i = 0; i < NumPlatforms; i++)
	{
		FVector RandomLocation(
			FMath::RandRange(-SpawnRange.X, SpawnRange.X),
			FMath::RandRange(-SpawnRange.Y, SpawnRange.Y),
			100.f
		);
		FRotator RandomRotation = FRotator::ZeroRotator;

		TSubclassOf<AActor> SelectedClass;
		int32 RandomType = FMath::RandRange(0, 2);

		if (RandomType == 0 && MovingPlatformClass)
			SelectedClass = MovingPlatformClass;
		else if (RandomType == 1 && RotatingPlatformClass)
			SelectedClass = RotatingPlatformClass;
		else if (TimerPlatformClass)
			SelectedClass = TimerPlatformClass;

		if (SelectedClass)
		{
			World->SpawnActor<AActor>(SelectedClass, RandomLocation, RandomRotation);
		}
	}
}
