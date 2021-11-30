// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ICollectable.h"
#include "Projectiles/BaseProjectile.h"
#include "BaseWeapon.generated.h"

UCLASS()
class JOYWAYTEST_API ABaseWeapon : public AActor, public IICollectable
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
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo");
	TSubclassOf<ABaseProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	int32 ClipSize = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmo = 50;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Grip(class UMotionControllerComponent* MotionController) override;

	UFUNCTION()
	virtual void Drop(class UMotionControllerComponent* MotionController) override;

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void Reload();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collectable")
	bool bIsHeld;

	bool bIsSimulatePhysics;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Collectable")
	class UMotionControllerComponent* CurrentMotionController;

private:
	int32 CurrentAmmo;
	int32 CurrentAmmoInClip;
	
};
