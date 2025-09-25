#include "TimerPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

ATimerPlatform::ATimerPlatform()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Game/Resources/Props/Floor_400x400.Floor_400x400")
	);
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(
		TEXT("/Game/Resources/Materials/M_Floor.M_Floor")
	);
	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
	}
}

void ATimerPlatform::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		ToggleTimerHandle, this, &ATimerPlatform::TogglePlatform, ToggleInterval, true
	);
}

void ATimerPlatform::TogglePlatform()
{
	bIsVisible = !bIsVisible;

	StaticMeshComp->SetVisibility(bIsVisible);
	StaticMeshComp->SetCollisionEnabled(
		bIsVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision
	);
}
