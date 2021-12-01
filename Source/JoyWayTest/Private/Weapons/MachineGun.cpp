// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MachineGun.h"

void AMachineGun::Fire()
{
	if (bIsReloading)
	{
		return;
	}
	bIsReadyToFire = true;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("WEAPON FIRE!!"));
	if (CurrentAmmoInClip > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireTimerHandle, this, &AMachineGun::AutoFire, AutoFireRate, bIsAutoFireLoop, AutoFireDelay);
	}
}

void AMachineGun::StopFire()
{
	bIsReadyToFire = false;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("STOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOPPPPPPPPPPPPPPPP"));
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
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("WEAPON FIRE!!"));
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
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("WEAPON FIRE!!"));
		FActorSpawnParameters Params;
		Params.Instigator = GetInstigator();
		Params.Owner = this;
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, Params);
	}

}
