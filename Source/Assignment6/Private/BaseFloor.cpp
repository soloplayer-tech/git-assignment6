#include "BaseFloor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"

ABaseFloor::ABaseFloor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	// 메쉬 적용
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Game/Resources/Props/Floor_400x400.Floor_400x400")
	);
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}

	// 머티리얼 적용
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(
		TEXT("/Game/Resources/Materials/M_Floor.M_Floor")
	);
	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
	}
}

void ABaseFloor::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	TargetLocation = StartLocation + MoveOffset;
}

void ABaseFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	FVector Destination = bMovingToTarget ? TargetLocation : StartLocation;

	FVector Direction = (Destination - CurrentLocation).GetSafeNormal();
	FVector NewLocation = CurrentLocation + Direction * MoveSpeed * DeltaTime;

	if (FVector::Dist(NewLocation, Destination) < 5.f)
	{
		bMovingToTarget = !bMovingToTarget;
		NewLocation = Destination;
	}

	SetActorLocation(NewLocation);
}
