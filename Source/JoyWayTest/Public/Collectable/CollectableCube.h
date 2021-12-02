// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable/CollectableBase.h"
#include "CollectableCube.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API ACollectableCube : public ACollectableBase
{
	GENERATED_BODY()

protected:
	ACollectableCube();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollider;
	
};
