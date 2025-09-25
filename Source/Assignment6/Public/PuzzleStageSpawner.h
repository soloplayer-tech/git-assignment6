#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleStageSpawner.generated.h"

UCLASS()
class ASSIGNMENT6_API APuzzleStageSpawner : public AActor
{
	GENERATED_BODY()

public:
	APuzzleStageSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> MovingPlatformClass;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> RotatingPlatformClass;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> TimerPlatformClass;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	int32 NumPlatforms = 5;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	FVector SpawnRange = FVector(2000.f, 2000.f, 0.f);
};
