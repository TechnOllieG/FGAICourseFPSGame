#pragma once
#include "Components/ActorComponent.h"

#include "MotionSensingComponent.generated.h"

class UAITurretGameInstance;

USTRUCT(BlueprintType)
struct FMotionSensingData
{
	GENERATED_BODY()

	FMotionSensingData() {};
	FMotionSensingData(AActor* Actor) { SensedActor = Actor; }

	UPROPERTY(BlueprintReadOnly)
	AActor* SensedActor;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMotionDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMotionResponseDelegate, const FMotionSensingData&, SensingData);

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AITURRETGAME_API UMotionSensingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMotionSensingComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Enabled = true;

	/** How often this motion sensing component should probe for motion */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PulsingFrequency = 1.f;

	/** Motion Targets will only be probed if they are within this range */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PulseRangeRadius = 1000.f;

	/** Show green sphere to visualize the pulse range */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool DebugRange = false;

	/** If the target is moving at this velocity or higher, it will be sensed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VelocityThreshold;

	UPROPERTY(BlueprintAssignable)
	FMotionDelegate OnPulse;

	UPROPERTY(BlueprintAssignable)
	FMotionResponseDelegate OnSenseTarget;

private:
	float LastPulseTime = -100.f;
	UAITurretGameInstance* GameInstance;
};