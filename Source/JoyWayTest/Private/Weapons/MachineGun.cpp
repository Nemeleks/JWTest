// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MachineGun.h"

void AMachineGun::Fire()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("WEAPON FIRE!!"));
	if (CurrentAmmoInClip > 0)
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
