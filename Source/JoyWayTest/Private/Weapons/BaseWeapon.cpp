// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeapon.h"

#include "MotionControllerComponent.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(MeshComponent);

	GrabComponent = CreateDefaultSubobject<USceneComponent>(TEXT("GrabComponent"));
	GrabComponent->SetupAttachment(MeshComponent);

	bIsHeld = false;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo-ClipSize;
	CurrentAmmoInClip = ClipSize;
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHeld)
	{
		if (CurrentAmmoInClip == 0)
		{
			Reload();
		}
	}
}

void ABaseWeapon::Grip(UMotionControllerComponent* MotionController)
{
	if (!bIsHeld)
	{
		bIsHeld = true;

		CurrentMotionController = MotionController;

		MeshComponent->SetSimulatePhysics(false);
		MeshComponent->AttachToComponent(MotionController, FAttachmentTransformRules::KeepWorldTransform);
		MeshComponent->SetRelativeRotation(GrabComponent->GetRelativeRotation());
		MeshComponent->SetWorldLocation(MotionController->GetComponentLocation() + (GrabComponent->GetComponentLocation() - MeshComponent->GetComponentLocation()));
	}
}

void ABaseWeapon::Drop(UMotionControllerComponent* MotionController)
{
	if ((MotionController == nullptr) ||(CurrentMotionController == MotionController))
	{
		bIsHeld = false;
		
		MeshComponent->SetSimulatePhysics(true);
		CurrentMotionController = nullptr;
		MeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void ABaseWeapon::Fire()
{

}

void ABaseWeapon::StopFire()
{
}

void ABaseWeapon::DisablePhysics()
{
	MeshComponent->SetSimulatePhysics(false);
}

void ABaseWeapon::Reload()
{
	if (CurrentAmmo < 1)
	{
		return;
	}
	int32 AmmoToAdd = ClipSize - CurrentAmmoInClip;
	if (CurrentAmmo < AmmoToAdd)
	{
		AmmoToAdd = CurrentAmmo;
	}
	CurrentAmmoInClip += AmmoToAdd;
	CurrentAmmo -= AmmoToAdd;
	bIsReloading = true;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ABaseWeapon::ReloadTimer,ReloadRate, bIsLoopReload, ReloadDelay);
}

void ABaseWeapon::ReloadTimer()
{
	bIsReloading = false;
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}
