// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Collectable.h"
#include "CollectableBase.generated.h"

UCLASS(Abstract)
class JOYWAYTEST_API ACollectableBase : public AActor, public ICollectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComponent;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Grip(class UMotionControllerComponent* MotionController) override;

	UFUNCTION()
	virtual void Drop(class UMotionControllerComponent* MotionController) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collectable")
	bool bIsHeld;

	bool bIsSimulatePhysics;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Collectable")
	class UMotionControllerComponent* CurrentMotionController;
};
