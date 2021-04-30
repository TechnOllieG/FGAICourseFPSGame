#pragma once
#include "GameFramework/Pawn.h"
#include "TurretGamePlayer.generated.h"

class USoundTargetComponent;
class UCapsuleComponent;
class UCameraComponent;

DECLARE_LOG_CATEGORY_EXTERN(TurretGamePlayer, Log, All);

UCLASS(Abstract)
class ATurretGamePlayer : public APawn
{
	GENERATED_BODY()

public:
	ATurretGamePlayer();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundTargetComponent* SoundTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AccelerationMultiplier = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AccelerationSpeed = 2000.f;

	UPROPERTY(EditDefaultsOnly)
	float Friction = 4.5f;

	UPROPERTY(EditDefaultsOnly)
	float JumpImpulse = 500.f;

	UPROPERTY(EditDefaultsOnly)
	float JumpCooldown = 0.5f;

	UPROPERTY(BlueprintReadOnly)
	bool IsCrouching = false;

	UPROPERTY(EditDefaultsOnly)
	float CapsuleHalfHeightCrouch;

	UPROPERTY(EditDefaultsOnly)
	float CrouchSpeed;

	UPROPERTY(EditDefaultsOnly)
	float AccelerationSpeedWhileCrouched = 1500.f;

	/** The maximum amount of iterations movement will take to slide against wall */
	UPROPERTY(EditDefaultsOnly)
	int MovementIterativeDepth = 4;

	/** How often (in seconds) the footstep sound should be broadcast to sound sensors, set to -1 for every frame */
	UPROPERTY(EditDefaultsOnly)
	float FootstepSoundDelay = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float WalkingFootstepLoudness = 100.f;

	UPROPERTY(EditDefaultsOnly)
	float CrouchingFootstepLoudness = 50.f;

	/** The trace channel to be used for the linetrace to check if player is grounded */
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ECollisionChannel> GroundTraceChannel;

	UPROPERTY(EditDefaultsOnly)
	float InteractionRange = 300.f;
	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel;

	UPROPERTY(EditDefaultsOnly)
	float Gravity = 981.f;

	UFUNCTION(BlueprintCallable)
	bool IsGrounded();

private:
	void HandleMoveForwardInput(float Value);
	void HandleMoveRightInput(float Value);
	void HandleLookUpInput(float Value);
	void HandleLookRightInput(float Value);
	void HandlePressJumpInput();
	void HandleReleaseJumpInput();
	void HandlePressCrouchInput();
	void HandleReleaseCrouchInput();
	void HandleInteractInputDown();
	
	void HandleCrouch(float DeltaTime);

	FVector Velocity = FVector::ZeroVector;
	float ForwardInput = 0.f;
	float TimeOfLastJump = -100.f;
	bool JumpInput = false;
	float RightInput = 0.f;
	float DefaultCapsuleHalfHeight = 0.f;
	float BaseAccelerationSpeed = 0.f;
	float LastFootstepTime = -1000.f;
};
