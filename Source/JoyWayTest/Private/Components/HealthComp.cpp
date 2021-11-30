// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComp.h"

// Sets default values for this component's properties
UHealthComp::UHealthComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComp::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	// ...
	
}

void UHealthComp::TakeDamage(float Amount)
{
	CurrentHealth -= Amount;
	if (CurrentHealth <= 0.f)
	{
		OnDie.Broadcast();
	}
	else
	{
		OnHealthChanged.Broadcast(Amount);
	}
}


