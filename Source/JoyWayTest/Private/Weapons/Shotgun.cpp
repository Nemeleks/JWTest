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
	Super::Fire();

	if (CurrentAmmoInClip > 0)
	{
		CurrentAmmoInClip -= 1;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("WEAPON FIRE!!"));
		FActorSpawnParameters Params;
		Params.Instigator = GetInstigator();
		Params.Owner = this;
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		for (int32 i = 0; i <= NumberOfBullets; ++i)
		{
			FVector RandDirection = FMath::VRandCone(ProjectileSpawnPoint->GetForwardVector(), 0.07f);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *RandDirection.ToString());
			ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass,SpawnLocation, SpawnRotation, Params);
			Projectile->FireInDirection(RandDirection);
		}

	}
}
