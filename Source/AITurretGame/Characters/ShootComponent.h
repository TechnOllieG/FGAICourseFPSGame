#pragma once
#include "Components/SceneComponent.h"
#include "ShootComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShootDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShootOnHitDelegate, const FHitResult&, HitResult);

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class UShootComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UShootComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Time to wait before firing the first shot after calling StartShooting */
	UPROPERTY(EditAnywhere)
	float InitialShootingCooldown;
	
	/** The cooldown between each shot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ShootingCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxShootDistance = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamagePerShot;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> ShootTraceChannel;

	UPROPERTY(BlueprintAssignable)
	FShootDelegate OnShoot;

	UPROPERTY(BlueprintAssignable)
	FShootOnHitDelegate OnHit;

	UFUNCTION(BlueprintCallable)
	void StartShooting();

	UFUNCTION(BlueprintCallable)
	void StopShooting();

	UFUNCTION(BlueprintCallable)
	float GetTimeToNextShot();

private:
	void HandlePressShootInput();
	void HandleReleaseShootInput();

	bool IsShooting = false;
	float TimeOfLastShot = -100.f;
	float StartShootingTime = -100.f;
};