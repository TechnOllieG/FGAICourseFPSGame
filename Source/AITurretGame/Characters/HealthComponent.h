#pragma once
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDamageDelegate, float, CurrentHealth);

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AITURRETGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 20.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentHealth = 0.f;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	void ResetHealth();
	
	UPROPERTY(BlueprintAssignable)
	FOnDeathDelegate OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnTakeDamageDelegate OnTakeDamage;

private:
	bool IsDead = false;
};
