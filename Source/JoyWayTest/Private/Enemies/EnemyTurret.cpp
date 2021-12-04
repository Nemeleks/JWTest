// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyTurret.h"

#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComp.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerVRCharacter.h"
#include "Weapons/BaseWeapon.h"

// Sets default values
AEnemyTurret::AEnemyTurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
		RootComponent = Root;
	}

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(MeshComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComp>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddDynamic(this, &AEnemyTurret::OnDie);

	TargetingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TeargetingSphere"));
	TargetingSphere->SetupAttachment(RootComponent);
	TargetingSphere->SetSphereRadius(TargetingSphereRadius, true);

	WeaponSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("WeaponSpawnPoint"));
	WeaponSpawnPoint->SetupAttachment(MeshComponent);

}

// Called when the game starts or when spawned
void AEnemyTurret::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld())
	{
		return;
	}

	Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, WeaponSpawnPoint->GetComponentLocation(), WeaponSpawnPoint->GetComponentRotation());
	Weapon->DisablePhysics();
	Weapon->AttachToComponent(WeaponSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	PlayerPawn = Cast<APlayerVRCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
}

// Called every frame
void AEnemyTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn->IsAlive())
	{
		Targeting();
	}

}

void AEnemyTurret::ApplyDamage(float DamageAmount)
{
	HealthComponent->TakeDamage(DamageAmount);
}

void AEnemyTurret::OnDie()
{
	if (Weapon)
	{
		Weapon->Destroy();
		Weapon = nullptr;
	}
	Destroy();
}

void AEnemyTurret::Targeting()
{
	if (IsPlayerInRange())
	{
		RotateToPlayer();
	}

	if (Weapon && IsReadyToFire())
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AEnemyTurret::Fire, FireRate, true, -1);
		}
	}
}

bool AEnemyTurret::IsPlayerInRange()
{
	FHitResult HitResult;
	FVector TraceStart = Weapon->GetProjectileSpawnPointLocation();
	FVector TraceEnd = PlayerPawn->GetActorLocation();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Turret Vission Trace")), true, Weapon);
	TraceParams.bReturnPhysicalMaterial = false;

//	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, -1, 0, 2);
	
	TArray<AActor*> OverlappedActors;
	TargetingSphere->GetOverlappingActors(OverlappedActors, PlayerPawnClass);
	
	
	if (OverlappedActors.Contains(PlayerPawn) && PlayerPawn->IsAlive())
	{
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Pawn, TraceParams))
		{
		
			if (HitResult.Actor == PlayerPawn)
			{
				return true;
			}
		}
	}
	
	if (GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
		Weapon->StopFire();
	}

	return false;
}

void AEnemyTurret::RotateToPlayer()
{
	if (!PlayerPawn)
	{
		return ;
	}
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrRotation = GetActorRotation();
	TargetRotation.Pitch = CurrRotation.Pitch;
	TargetRotation.Roll = CurrRotation.Roll;
	SetActorRotation(FMath::RInterpConstantTo(CurrRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
}

bool AEnemyTurret::IsReadyToFire()
{
	if (!PlayerPawn)
	{
		return false;
	}
	FVector TargetingDir = GetActorForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDir, DirToPlayer)));
	if (AimAngle <= Accuracy)
	{
		return true;
	}
	return false;
}

void AEnemyTurret::Fire()
{
	if ((Weapon->GetCurrentAmmoInClip() == 0) || (!PlayerPawn->IsAlive()))
	{
		Weapon->Reload();
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}
	else
	{
		if (Weapon)
		{
			Weapon->Fire();
		}
	}
	
}

