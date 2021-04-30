#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetHealth();
}

void UHealthComponent::TakeDamage(float Damage)
{
	if(IsDead)
		return;
	
	if(CurrentHealth - Damage <= 0.f)
	{
		IsDead = true;
		CurrentHealth = 0.f;
		OnTakeDamage.Broadcast(CurrentHealth);
		OnDeath.Broadcast();
		return;
	}
	CurrentHealth -= Damage;

	OnTakeDamage.Broadcast(CurrentHealth);
}

void UHealthComponent::ResetHealth()
{
	CurrentHealth = MaxHealth;
	IsDead = false;
}