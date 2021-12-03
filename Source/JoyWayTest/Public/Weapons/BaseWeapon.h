// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectiles/BaseProjectile.h"
#include "Components/ArrowComponent.h"
#include "Interfaces/Collectable.h"
#include "BaseWeapon.generated.h"

UCLASS(Abstract)
class JOYWAYTEST_API ABaseWeapon : public AActor, public ICollectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components");
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components");
	USceneComponent* GrabComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components");
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo");
	TSubclassOf<ABaseProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	int32 ClipSize = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmo = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Reload")
	float ReloadRate = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Reload")
	float ReloadDelay = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Reload")
	bool bIsLoopReload = false;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Grip(class UMotionControllerComponent* MotionController) override;

	UFUNCTION()
	virtual void Drop(class UMotionControllerComponent* MotionController) override;

	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	virtual void StopFire();

	void DisablePhysics();

	UFUNCTION()
	void Reload();

	UFUNCTION(BlueprintCallable)
	float GetCurrentAmmo() const {return CurrentAmmo;}

	UFUNCTION(BlueprintCallable)
	float GetCurrentAmmoInClip() const {return CurrentAmmoInClip;}

	FVector GetProjectileSpawnPointLocation() const {return ProjectileSpawnPoint->GetComponentLocation();}
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collectable")
	bool bIsHeld;

	bool bIsSimulatePhysics;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Collectable")
	class UMotionControllerComponent* CurrentMotionController;

	int32 CurrentAmmo;
	int32 CurrentAmmoInClip;

	FTimerHandle ReloadTimerHandle;
	bool bIsReloading = false;

	void ReloadTimer();

private:

	
};
