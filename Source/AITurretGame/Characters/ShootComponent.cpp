#include "ShootComponent.h"

#include "HealthComponent.h"
#include "TurretGamePlayer.h"

UShootComponent::UShootComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UShootComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ATurretGamePlayer* Player = GetOwner<ATurretGamePlayer>();
	if(Player != nullptr)
	{
		Player->InputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &UShootComponent::HandlePressShootInput);
		Player->InputComponent->BindAction(TEXT("Shoot"), IE_Released, this, &UShootComponent::HandleReleaseShootInput);
	}
}

void UShootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	const float Time = GetOwner()->GetGameTimeSinceCreation();
	
	if(IsShooting && Time - StartShootingTime > InitialShootingCooldown && Time - TimeOfLastShot > ShootingCooldown)
	{
		TimeOfLastShot = Time;
		OnShoot.Broadcast();
		
		FHitResult Hit;
		
		FVector Origin = GetComponentLocation();
		FVector Destination = Origin + MaxShootDistance * GetForwardVector();
		
		if(GetWorld()->LineTraceSingleByChannel(Hit, Origin, Destination, ShootTraceChannel))
		{
			OnHit.Broadcast(Hit);
			UHealthComponent* HealthComp = Hit.GetActor()->FindComponentByClass<UHealthComponent>();
			if(HealthComp != nullptr)
			{
				HealthComp->TakeDamage(DamagePerShot);
			}
		}
	}
}

void UShootComponent::StartShooting()
{
	if(IsShooting)
		return;
	
	IsShooting = true;
	StartShootingTime = GetOwner()->GetGameTimeSinceCreation();
}

void UShootComponent::StopShooting()
{
	if(!IsShooting)
		return;
	
	IsShooting = false;
}

float UShootComponent::GetTimeToNextShot()
{
	const float Time = GetOwner()->GetGameTimeSinceCreation();
	return ShootingCooldown - (Time - TimeOfLastShot);
}

void UShootComponent::HandlePressShootInput()
{
	StartShooting();
}

void UShootComponent::HandleReleaseShootInput()
{
	StopShooting();
}
