// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API AShotgun : public ABaseWeapon
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	float ConeHalfAngleRad = 0.07f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	int32 NumberOfBullets = 50;
public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void Fire() override;
};
