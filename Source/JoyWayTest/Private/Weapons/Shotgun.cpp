// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Shotgun.h"

void AShotgun::BeginPlay()
{
	Super::BeginPlay();
}

void AShotgun::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AShotgun::Fire()
{

	if (bIsReloading)
	{
		return;
	}
	
	if (CurrentAmmoInClip > 0)
	{
		CurrentAmmoInClip -= 1;
		FActorSpawnParameters Params;
		Params.Instigator = GetInstigator();
		Params.Owner = this;
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		for (int32 i = 0; i < NumberOfBullets; ++i)
		{
			FVector RandDirection = FMath::VRandCone(ProjectileSpawnPoint->GetForwardVector(), ConeHalfAngleRad);
			ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass,SpawnLocation, SpawnRotation, Params);
			Projectile->FireInDirection(RandDirection);
		}
	}
}
