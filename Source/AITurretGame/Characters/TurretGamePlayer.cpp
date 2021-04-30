#include "TurretGamePlayer.h"

#include "AITurretGame/AI/Sensing/SoundTargetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "AITurretGame/InteractionSystem/TurretGameButton.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(TurretGamePlayer);

ATurretGamePlayer::ATurretGamePlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	RootComponent = Capsule;
	SoundTarget = CreateDefaultSubobject<USoundTargetComponent>(TEXT("Sound Target Component"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

void ATurretGamePlayer::BeginPlay()
{
	Super::BeginPlay();
	DefaultCapsuleHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();
	BaseAccelerationSpeed = AccelerationSpeed;
}

void ATurretGamePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATurretGamePlayer::HandleMoveForwardInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATurretGamePlayer::HandleMoveRightInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATurretGamePlayer::HandleLookUpInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ATurretGamePlayer::HandleLookRightInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATurretGamePlayer::HandlePressJumpInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ATurretGamePlayer::HandleReleaseJumpInput);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ATurretGamePlayer::HandlePressCrouchInput);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &ATurretGamePlayer::HandleReleaseCrouchInput);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ATurretGamePlayer::HandleInteractInputDown);
}

void ATurretGamePlayer::HandleMoveForwardInput(float Value)
{
	ForwardInput = Value;
}

void ATurretGamePlayer::HandleMoveRightInput(float Value)
{
	RightInput = Value;
}

void ATurretGamePlayer::HandleLookUpInput(float Value)
{
	AddControllerPitchInput(Value);
}

void ATurretGamePlayer::HandleLookRightInput(float Value)
{
	AddControllerYawInput(Value);
}

void ATurretGamePlayer::HandlePressJumpInput()
{
	JumpInput = true;
}

void ATurretGamePlayer::HandleReleaseJumpInput()
{
	JumpInput = false;
}

void ATurretGamePlayer::HandlePressCrouchInput()
{
	IsCrouching = true;
}

void ATurretGamePlayer::HandleReleaseCrouchInput()
{
	IsCrouching = false;
}

void ATurretGamePlayer::HandleInteractInputDown()
{
	FHitResult Hit;
	FVector Origin = Camera->GetComponentLocation();
	FVector Destination = Origin + Camera->GetForwardVector() * InteractionRange;
	
	if(GetWorld()->LineTraceSingleByChannel(Hit, Origin, Destination, InteractionTraceChannel))
	{
		ATurretGameButton* Button = Cast<ATurretGameButton>(Hit.GetActor());
		if(Button != nullptr)
		{
			Button->OnPress(this);
		}
		else
		{
			UE_LOG(TurretGamePlayer, Warning, TEXT("Tried to interact with something that isn't interactable. Make sure that the interaction trace channel only blocks interactables"));
		}
	}
}

bool ATurretGamePlayer::IsGrounded()
{
	FHitResult Hit;
	const FVector CapsuleExtent = Capsule->Bounds.BoxExtent;
	const float LineTraceLength = CapsuleExtent.Z + 0.2f;

	FVector Origin = GetActorLocation();
	FVector End = Origin + FVector::DownVector * LineTraceLength;

	// GetWorld()->LineTraceSingleByChannel(Hit, Origin, End, GroundTraceChannel.GetValue());
	// return Hit.bBlockingHit;
	
	const FVector ForwardOffset = FVector::ForwardVector * CapsuleExtent.X;
	const FVector RightOffset = FVector::RightVector * CapsuleExtent.Y;
	
	for(int i = 0; i < 5; i++)
	{
		GetWorld()->LineTraceSingleByChannel(Hit, Origin, End, GroundTraceChannel.GetValue());
		if(Hit.bBlockingHit)
			return true;
		
		switch(i)
		{
			case 0:
				{
					Origin += ForwardOffset;
					End += ForwardOffset;
					break;
				}
			case 1:
				{
					Origin -= ForwardOffset * 2.f;
					End -= ForwardOffset * 2.f;
					break;
				}
			case 2:
				{
					Origin += ForwardOffset + RightOffset;
					End += ForwardOffset + RightOffset;
					break;
				}
			case 3:
				{
					Origin -= RightOffset * 2.f;
					End -= RightOffset * 2.f;
					break;
				}
			case 4:
				{
					break;
				}
			default:
				UE_LOG(LogTemp, Warning, TEXT("This shouldn't be possible (in TurretGamePlayer.cpp)"))
				return true;
		}
	}
	
	return false;
}

void ATurretGamePlayer::HandleCrouch(float DeltaTime)
{
	const float TargetHalfHeight = IsCrouching ? CapsuleHalfHeightCrouch : DefaultCapsuleHalfHeight;
	const float CurrentHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();

	const float TargetAccelerationSpeed = IsCrouching ? AccelerationSpeedWhileCrouched * AccelerationMultiplier : BaseAccelerationSpeed * AccelerationMultiplier;

	if(FMath::IsNearlyZero(TargetHalfHeight - CurrentHalfHeight) && FMath::IsNearlyZero(AccelerationSpeed - TargetAccelerationSpeed))
		return;

	const float NewHalfHeight = UKismetMathLibrary::Lerp(CurrentHalfHeight, TargetHalfHeight, CrouchSpeed * DeltaTime);
	AccelerationSpeed = UKismetMathLibrary::Lerp(AccelerationSpeed, TargetAccelerationSpeed, CrouchSpeed * DeltaTime);
	
	AddActorLocalOffset(FVector::UpVector * (NewHalfHeight - CurrentHalfHeight));
	Capsule->SetCapsuleHalfHeight(NewHalfHeight);
}

void ATurretGamePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const bool Grounded = IsGrounded();

	HandleCrouch(DeltaTime);
	
	const FVector Acceleration = (ForwardInput * GetActorForwardVector() + RightInput * GetActorRightVector()).GetSafeNormal() * AccelerationSpeed;
	Velocity += (Acceleration - Velocity * Friction) * DeltaTime;
	if(!Grounded)
		Velocity -= Gravity * FVector::UpVector * DeltaTime;

	const float GameTime = GetGameTimeSinceCreation();
	if(Grounded && GameTime - LastFootstepTime > FootstepSoundDelay && (!FMath::IsNearlyZero(ForwardInput) || !FMath::IsNearlyZero(RightInput)))
	{
		LastFootstepTime = GameTime;
		if(IsCrouching)
		{
			SoundTarget->PlaySound(CrouchingFootstepLoudness);
		}
		else
		{
			SoundTarget->PlaySound(WalkingFootstepLoudness);
		}
	}

	if(JumpInput && Grounded && GameTime - TimeOfLastJump > JumpCooldown)
	{
		TimeOfLastJump = GameTime;
		Velocity += GetActorUpVector() * JumpImpulse;
	}

	FVector DeltaToMove = Velocity * DeltaTime;
	
	FHitResult Hit;

	for(int i = 0; i < MovementIterativeDepth; i++)
	{
		AddActorWorldOffset(DeltaToMove, true, &Hit);

		DeltaToMove -= DeltaToMove * Hit.Time;
		
		if(Hit.bBlockingHit)
		{
			const FVector DeprenetationDelta = FVector::DotProduct(Hit.Normal, Velocity) * Hit.Normal;
			Velocity -= DeprenetationDelta;
			
			if(DeltaToMove.IsNearlyZero())
				break;
			
			DeltaToMove -= FVector::DotProduct(DeltaToMove, Hit.Normal) * Hit.Normal;
		}
	}
}
