// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MachineGun.h"

void AMachineGun::Fire()
{
	if (bIsReloading)
	{
		return;
	}
	bIsReadyToFire = true;
	if (CurrentAmmoInClip > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireTimerHandle, this, &AMachineGun::AutoFire, AutoFireRate, bIsAutoFireLoop, AutoFireDelay);
	}
}

void AMachineGun::StopFire()
{
	bIsReadyToFire = false;
}

void AMachineGun::AutoFire()
{
	if (!bIsReadyToFire)
	{
		return;
	}
	if (CurrentAmmoInClip == 1)
	{
		CurrentAmmoInClip -= 1;
		FActorSpawnParameters Params;
		Params.Instigator = GetInstigator();
		Params.Owner = this;
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, Params);
		GetWorld()->GetTimerManager().ClearTimer(AutoFireTimerHandle);
	}
	else
	{
		CurrentAmmoInClip -= 1;
		FActorSpawnParameters Params;
		Params.Instigator = GetInstigator();
		Params.Owner = this;
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, Params);
	}

}
