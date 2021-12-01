// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Damageable.h"
#include "ShootingTarget.generated.h"

UCLASS()
class JOYWAYTEST_API AShootingTarget : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShootingTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* Collider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComp* HealthComp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void ApplyDamage(float DamageAmount) override;

	UFUNCTION()
	void OnDie();

	UFUNCTION()
	void OnHealthChanged(float DamageAmount);
	
};

