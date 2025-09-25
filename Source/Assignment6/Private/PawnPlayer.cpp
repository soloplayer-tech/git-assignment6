#include "PawnPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "PawnPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

APawnPlayer::APawnPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    // Root capsule
    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    CapsuleComp->InitCapsuleSize(34.f, 88.f);
    CapsuleComp->SetSimulatePhysics(false);
    RootComponent = CapsuleComp;

    // Skeletal Mesh
    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetSimulatePhysics(false);

    // SpringArm + Camera
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 300.f;
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;

    // Movement
    MoveSpeed = 600.f;
    RotateSpeed = 100.f;
    Deceleration = 8.f;
    JumpStrength = 420.f;

    // Gravity
    Gravity = -980.f;
    Velocity = FVector::ZeroVector;

    bJumpRequested = false;
    bIsGrounded = false;
}

void APawnPlayer::BeginPlay()
{
    Super::BeginPlay();
}

void APawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (APawnPlayerController* PC = Cast<APawnPlayerController>(GetController()))
        {
            if (PC->MoveAction)
                EnhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Triggered, this, &APawnPlayer::Move);

            if (PC->LookAction)
                EnhancedInput->BindAction(PC->LookAction, ETriggerEvent::Triggered, this, &APawnPlayer::Look);

            if (PC->JumpAction)
            {
                EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Triggered, this, &APawnPlayer::JumpPressed);
                EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Completed, this, &APawnPlayer::JumpReleased);
            }
        }
    }
}

void APawnPlayer::Move(const FInputActionValue& Value)
{
    MoveInput = Value.Get<FVector2D>();
}

void APawnPlayer::Look(const FInputActionValue& Value)
{
    LookInput = Value.Get<FVector2D>();
}

void APawnPlayer::JumpPressed(const FInputActionValue& Value)
{
    bJumpRequested = true;
}

void APawnPlayer::JumpReleased(const FInputActionValue& Value)
{
    bJumpRequested = false;
}

void APawnPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ApplyGravity(DeltaTime);
    HandleJump();
    ApplyMovement(DeltaTime);

    // Rotation
    FRotator NewRot = GetActorRotation();
    NewRot.Yaw += LookInput.X * RotateSpeed * DeltaTime;
    SetActorRotation(NewRot);

    // Pitch rotation on SpringArm
    FRotator ArmRot = SpringArmComp->GetRelativeRotation();
    ArmRot.Pitch = FMath::Clamp(ArmRot.Pitch + LookInput.Y * RotateSpeed * DeltaTime, -80.f, 80.f);
    SpringArmComp->SetRelativeRotation(ArmRot);
}

void APawnPlayer::ApplyGravity(float DeltaTime)
{
    FVector Start = GetActorLocation();
    FVector End = Start - FVector(0, 0, CapsuleComp->GetScaledCapsuleHalfHeight() + 2.f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params))
    {
        bIsGrounded = true;
        Velocity.Z = 0.f;
        SetActorLocation(FVector(Start.X, Start.Y, Hit.ImpactPoint.Z + CapsuleComp->GetScaledCapsuleHalfHeight()));
    }
    else
    {
        bIsGrounded = false;
        Velocity.Z += Gravity * DeltaTime;
    }
}

void APawnPlayer::HandleJump()
{
    if (bJumpRequested && bIsGrounded)
    {
        Velocity.Z = JumpStrength;
        bIsGrounded = false;
    }
}

void APawnPlayer::ApplyMovement(float DeltaTime)
{
    if (!Controller) return;

    FVector Forward = GetActorForwardVector();
    FVector Right = GetActorRightVector();

    FVector TargetVel = (Forward * MoveInput.Y + Right * MoveInput.X).GetClampedToMaxSize(1.f) * MoveSpeed;

    // 자연스러운 감속 (Lerp)
    Velocity.X = FMath::FInterpTo(Velocity.X, TargetVel.X, DeltaTime, Deceleration);
    Velocity.Y = FMath::FInterpTo(Velocity.Y, TargetVel.Y, DeltaTime, Deceleration);

    FVector Delta = FVector(Velocity.X, Velocity.Y, Velocity.Z) * DeltaTime;
    AddActorWorldOffset(Delta, true);
}
