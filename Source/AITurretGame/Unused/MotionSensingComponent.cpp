#include "MotionSensingComponent.h"

#include "MotionTargetComponent.h"
#include "AITurretGame/AITurretGameInstance.h"
#include "Kismet/GameplayStatics.h"

UMotionSensingComponent::UMotionSensingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMotionSensingComponent::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UAITurretGameInstance>(UGameplayStatics::GetGameInstance(this));
}

void UMotionSensingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Location = GetOwner()->GetActorLocation();

	if(DebugRange)
		UKismetSystemLibrary::DrawDebugSphere(this, Location, PulseRangeRadius, 12, FLinearColor::Green);
	
	if(!Enabled)
		return;
	
	float Time = GetOwner()->GetGameTimeSinceCreation();

	if(Time - LastPulseTime > PulsingFrequency)
	{
		OnPulse.Broadcast();

		for(UMotionTargetComponent* TargetComp : GameInstance->MotionTargetComponents)
		{
			if(TargetComp->CurrentSquaredVelocity >= FMath::Square(VelocityThreshold))
			{
				FMotionSensingData Data = FMotionSensingData(TargetComp->GetOwner());
				OnSenseTarget.Broadcast(Data);
			}
		}
	}
}

