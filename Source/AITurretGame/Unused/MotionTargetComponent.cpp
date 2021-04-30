#include "MotionTargetComponent.h"

#include "AITurretGame/AITurretGameInstance.h"
#include "Kismet/GameplayStatics.h"

UMotionTargetComponent::UMotionTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMotionTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	LastFramesPosition = GetOwner()->GetActorLocation();
	GameInstance = Cast<UAITurretGameInstance>(UGameplayStatics::GetGameInstance(this));
	GameInstance->MotionTargetComponents.Add(this);
}

void UMotionTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const FVector CurrentPosition = GetOwner()->GetActorLocation();

	CurrentSquaredVelocity = (CurrentPosition - LastFramesPosition).SizeSquared() * DeltaTime;
	if(CalculateNonSquaredVelocity)
		CurrentVelocity = FMath::Sqrt(CurrentSquaredVelocity);
	
	LastFramesPosition = CurrentPosition;
}
