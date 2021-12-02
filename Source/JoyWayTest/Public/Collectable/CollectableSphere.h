// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable/CollectableBase.h"
#include "CollectableSphere.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API ACollectableSphere : public ACollectableBase
{
	GENERATED_BODY()

protected:
	ACollectableSphere();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USphereComponent* SphereCollider;
	
};
