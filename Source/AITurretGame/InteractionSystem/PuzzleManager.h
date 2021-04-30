#pragma once
#include "AITurretGame/AI/StateMachineComponent.h"
#include "Components/ActorComponent.h"
#include "PuzzleManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPuzzleCompleteDelegate);
DECLARE_LOG_CATEGORY_EXTERN(LogPuzzleManager, Log, All);

UCLASS(ClassGroup=(Gameplay), meta=(BlueprintSpawnableComponent))
class AITURRETGAME_API UPuzzleManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPuzzleManager();
	virtual void BeginPlay() override;

	/** The state machines that affect this puzzle, when all these have reached target state, on complete puzzle will be called */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AActor*> StateMachineActors;

	/** When all state machines have reached the state with this index, the on complete puzzle delegate will be called */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int TargetState;

	UPROPERTY(BlueprintAssignable)
	FPuzzleCompleteDelegate OnCompletePuzzle;

	UFUNCTION()
	void CheckConditions(UStateMachineComponent* CurrentMachine);
	
private:
	bool PuzzleIsSolved = false;
};