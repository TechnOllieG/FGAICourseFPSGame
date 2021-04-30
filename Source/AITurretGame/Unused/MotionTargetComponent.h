#pragma once
#include "Components/ActorComponent.h"
#include "MotionTargetComponent.generated.h"

class UAITurretGameInstance;
UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AITURRETGAME_API UMotionTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMotionTargetComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Current squared velocity, in units/s */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentSquaredVelocity = 0.f;

	/** If the actual current velocity should be calculated by performing a square root on the squared velocity (might be expensive).
	 * Only use this for debugging purposes or if you need to access the actual velocity in blueprint */
	UPROPERTY(EditAnywhere)
	bool CalculateNonSquaredVelocity = false;

	/** Current velocity, in units/s */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentVelocity = 0.f;

private:
	FVector LastFramesPosition;
	UAITurretGameInstance* GameInstance;
};
