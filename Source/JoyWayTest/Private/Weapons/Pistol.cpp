// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Pistol.h"

void APistol::BeginPlay()
{
	Super::BeginPlay();
}

void APistol::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APistol::Fire()
{
	Super::Fire();
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
		GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, Params);
	}
}
