#pragma once
#include "Components/ActorComponent.h"
#include "SplineFollower.generated.h"

class USplineComponent;
UENUM(BlueprintType)
enum ESplineFollowMode
{
	Looped,
	Pendulum
};

DECLARE_LOG_CATEGORY_EXTERN(LogSplineFollower, Log, All);

UCLASS(ClassGroup=(Tools), meta=(BlueprintSpawnableComponent))
class AITURRETGAME_API USplineFollower : public UActorComponent
{
	GENERATED_BODY()

public:
	USplineFollower();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	AActor* ActorWithSplineComponent;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ESplineFollowMode> SplineFollowMode;

	/** If the object's scale should change according to the scale of the points on the spline */
	UPROPERTY(EditAnywhere)
	bool UseScale = false;

	/** The speed this actor should travel along the spline at (in spline distance units/second) */
	UPROPERTY(EditAnywhere)
	float TravelSpeed;

private:
	float Distance = 0.f;
	USplineComponent* SplineComponent;
	bool Reverse = false;
};