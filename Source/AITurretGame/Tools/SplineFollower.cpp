#include "SplineFollower.h"
#include "Components/SplineComponent.h"

DEFINE_LOG_CATEGORY(LogSplineFollower);

USplineFollower::USplineFollower()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USplineFollower::BeginPlay()
{
    Super::BeginPlay();

	if(ActorWithSplineComponent == nullptr)
	{
		UE_LOG(LogSplineFollower, Warning, TEXT("If you want the spline follower to follow something, please assign an actor."));
		return;
	}
	
	SplineComponent = ActorWithSplineComponent->FindComponentByClass<USplineComponent>();
	if(SplineComponent == nullptr)
		UE_LOG(LogSplineFollower, Warning, TEXT("Couldn't get spline component from actor in spline follower component attached to actor with name: %s"), *GetOwner()->GetName());
}

void USplineFollower::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(SplineComponent == nullptr)
		return;

	FTransform SplineTransform = SplineComponent->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World, UseScale);
	float MaxSplineDistance = SplineComponent->GetSplineLength();

	GetOwner()->SetActorTransform(SplineTransform);

	switch(SplineFollowMode.GetValue())
	{
	case Looped:
		{
			if(Distance > MaxSplineDistance)
				Distance = 0.f;
			
			Distance += DeltaTime * TravelSpeed;
			break;
		}
	case Pendulum:
		{
			if(Distance > MaxSplineDistance || Distance < 0.f)
				Reverse = !Reverse;

			if(Reverse)
				Distance -= DeltaTime * TravelSpeed;
			else
				Distance += DeltaTime * TravelSpeed;
			break;
		}
	default:
		UE_LOG(LogSplineFollower, Warning, TEXT("Forgot to add a case for a newly added SplineFollowMode"));
	}
}
