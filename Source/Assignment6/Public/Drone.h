#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Drone.generated.h"

UCLASS()
class ASSIGNMENT6_API ADrone : public APawn
{
    GENERATED_BODY()

public:
    ADrone();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    // Components
    UPROPERTY(VisibleAnywhere)
    class UCapsuleComponent* CapsuleComponent;

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere)
    class USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* Camera;

    // Input Actions (Controller에서 세팅한 자산을 Pawn에서 바인딩)
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* RollAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* UpDownAction;

    // Movement
    FVector Velocity;
    float GravityAcceleration = -980.f;
    bool bIsGrounded = false;

    float GroundSpeed = 500.f;
    float AirSpeed = 250.f;

    // Input handlers
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Roll(const FInputActionValue& Value);
    void MoveUpDown(const FInputActionValue& Value);
};