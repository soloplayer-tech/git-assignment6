#include "Drone.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"

ADrone::ADrone()
{
    PrimaryActorTick.bCanEverTick = true;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    RootComponent = CapsuleComponent;
    CapsuleComponent->SetSimulatePhysics(false);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetSimulatePhysics(false);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.f;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void ADrone::BeginPlay()
{
    Super::BeginPlay();
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADrone::Move);
        EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADrone::Look);
        EIC->BindAction(RollAction, ETriggerEvent::Triggered, this, &ADrone::Roll);
        EIC->BindAction(UpDownAction, ETriggerEvent::Triggered, this, &ADrone::MoveUpDown);
    }
}

void ADrone::Move(const FInputActionValue& Value)
{
    if (bIsGrounded) return;

    FVector2D Input = Value.Get<FVector2D>();
    FVector Forward = GetActorForwardVector() * Input.Y;
    FVector Right = GetActorRightVector() * Input.X;

    AddActorLocalOffset((Forward + Right) * AirSpeed * GetWorld()->GetDeltaSeconds(), true);
}

void ADrone::Look(const FInputActionValue& Value)
{
    FVector2D Input = Value.Get<FVector2D>();
    FRotator DeltaRot = FRotator(-Input.Y, Input.X, 0.f);
    AddActorLocalRotation(DeltaRot * 100.f * GetWorld()->GetDeltaSeconds());
}

void ADrone::Roll(const FInputActionValue& Value)
{
    float Input = Value.Get<float>();
    FRotator DeltaRot = FRotator(0.f, 0.f, Input);
    AddActorLocalRotation(DeltaRot * 100.f * GetWorld()->GetDeltaSeconds());
}

void ADrone::MoveUpDown(const FInputActionValue& Value)
{
    float Input = Value.Get<float>();
    if (Input > 0.f) bIsGrounded = false;

    float Speed = bIsGrounded ? GroundSpeed : AirSpeed;
    AddActorLocalOffset(FVector(0.f, 0.f, Input * Speed * GetWorld()->GetDeltaSeconds()), true);
}

void ADrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsGrounded)
    {
        Velocity.Z += GravityAcceleration * DeltaTime;
        AddActorLocalOffset(Velocity * DeltaTime, true);
    }

    // 지면 충돌 감지
    FVector Start = GetActorLocation();
    FVector End = Start - FVector(0.f, 0.f, 100.f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    if (bHit)
    {
        bIsGrounded = true;
        Velocity.Z = 0.f;
    }
}