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
	virtual void StopFire() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoFire")
	float AutoFireRate = 0.2f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoFire")
	float AutoFireDelay = -1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoFire")
	bool bIsAutoFireLoop = true;
	
private:
	bool bIsReadyToFire = true;

	FTimerHandle AutoFireTimerHandle;

	void AutoFire();

};
