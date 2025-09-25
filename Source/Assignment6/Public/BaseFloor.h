#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseFloor.generated.h"

UCLASS()
class ASSIGNMENT6_API ABaseFloor : public AActor
{
	GENERATED_BODY()

public:
	ABaseFloor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Settings")
	FVector MoveOffset = FVector(500.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Settings")
	float MoveSpeed = 100.f;

private:
	FVector StartLocation;
	FVector TargetLocation;
	bool bMovingToTarget = true;
};
