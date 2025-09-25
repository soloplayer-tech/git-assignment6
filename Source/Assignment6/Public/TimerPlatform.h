#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerPlatform.generated.h"

UCLASS()
class ASSIGNMENT6_API ATimerPlatform : public AActor
{
	GENERATED_BODY()

public:
	ATimerPlatform();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Settings")
	float ToggleInterval = 2.f;

private:
	FTimerHandle ToggleTimerHandle;
	bool bIsVisible = true;

	void TogglePlatform();
};
