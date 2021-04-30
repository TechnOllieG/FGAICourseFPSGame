#include "PuzzleManager.h"

DEFINE_LOG_CATEGORY(LogPuzzleManager);

UPuzzleManager::UPuzzleManager()
{
	
}

void UPuzzleManager::BeginPlay()
{
	Super::BeginPlay();
	
	for(AActor* Actor : StateMachineActors)
	{
		UStateMachineComponent* StateMachine = Actor->FindComponentByClass<UStateMachineComponent>();
		if(StateMachine == nullptr)
		{
			UE_LOG(LogPuzzleManager, Warning, TEXT("Actor referenced in puzzle manager attached to actor with name: %s does not contain a state machine, only reference actors with attached state machines"), *GetOwner()->GetName())
			return;
		}
		
		StateMachine->OnChangeState.AddDynamic(this, &UPuzzleManager::CheckConditions);
	}
}


void UPuzzleManager::CheckConditions(UStateMachineComponent* CurrentMachine)
{
	if(PuzzleIsSolved)
		return;
	
	for(AActor* Actor : StateMachineActors)
	{
		UStateMachineComponent* StateMachine = Actor->FindComponentByClass<UStateMachineComponent>();
		if(StateMachine == nullptr)
		{
			UE_LOG(LogPuzzleManager, Warning, TEXT("Actor referenced in puzzle manager attached to actor with name: %s does not contain a state machine, only reference actors with attached state machines"), *GetOwner()->GetName())
			return;
		}
		
		if(StateMachine->CurrentStateIndex != TargetState)
			return;
	}
	PuzzleIsSolved = true;
	OnCompletePuzzle.Broadcast();
}
