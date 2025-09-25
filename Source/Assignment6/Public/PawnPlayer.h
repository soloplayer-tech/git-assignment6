#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnPlayer.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class ASSIGNMENT6_API APawnPlayer : public APawn
{
    GENERATED_BODY()

public:
    APawnPlayer();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    // Components
    UPROPERTY(VisibleAnywhere)
    UCapsuleComponent* CapsuleComp;

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComp;

    // Input
    FVector2D MoveInput;
    FVector2D LookInput;
    bool bJumpRequested;

    UFUNCTION()
    void Move(const FInputActionValue& Value);

    UFUNCTION()
    void Look(const FInputActionValue& Value);

    UFUNCTION()
    void JumpPressed(const FInputActionValue& Value);

    UFUNCTION()
    void JumpReleased(const FInputActionValue& Value);

    // Movement
    FVector Velocity;
    float MoveSpeed;
    float RotateSpeed;
    float Deceleration; // XY °¨¼Ó
    float JumpStrength;
    bool bIsGrounded;

    // Gravity
    float Gravity;

    void ApplyGravity(float DeltaTime);
    void ApplyMovement(float DeltaTime);
    void HandleJump();
};
