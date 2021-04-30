#pragma once
#include "GameFramework/Actor.h"
#include "TurretGameButton.generated.h"

UCLASS(Abstract)
class AITURRETGAME_API ATurretGameButton : public AActor
{
	GENERATED_BODY()

public:
	ATurretGameButton();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPress(const AActor* ActorThatPressed);
};