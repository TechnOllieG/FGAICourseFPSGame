#pragma once
#include "Components/ActorComponent.h"
#include "SoundTargetComponent.generated.h"

class UAITurretGameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSoundDelegate);

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AITURRETGAME_API USoundTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoundTargetComponent();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void PlaySound(float Loudness);

	UPROPERTY(BlueprintAssignable)
	FSoundDelegate OnSoundPlay;

private:
	UAITurretGameInstance* GameInstance;
};