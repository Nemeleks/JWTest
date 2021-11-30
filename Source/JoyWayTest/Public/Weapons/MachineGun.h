// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "MachineGun.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API AMachineGun : public ABaseWeapon
{
	GENERATED_BODY()
public:
	virtual void Fire() override;
};
