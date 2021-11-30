// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API APistol : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void Fire() override;
};
