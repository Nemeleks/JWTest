// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerVRCharacter.h"

#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/HealthComp.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Weapons/BaseWeapon.h"

// Sets default values
APlayerVRCharacter::APlayerVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VrRoot"));
	VRRoot->SetupAttachment(RootComponent);

	LeftMoController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMoController"));
	LeftMoController->SetupAttachment(VRRoot);
	LeftMoController->SetTrackingSource(EControllerHand::Left);

	LeftInteractionWidget = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("LeftInteractionWidget"));
	LeftInteractionWidget->SetupAttachment(LeftMoController);

	LeftCollider = CreateDefaultSubobject<USphereComponent>(TEXT("LeftCollider"));
	LeftCollider->SetupAttachment(LeftMoController);
	LeftCollider->SetSphereRadius(5.f, true);

	RightMoController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMoController"));
	RightMoController->SetupAttachment(VRRoot);
	RightMoController->SetTrackingSource(EControllerHand::Right);

	RightCollider = CreateDefaultSubobject<USphereComponent>(TEXT("RightCollider"));
	RightCollider->SetupAttachment(RightMoController);
	RightCollider->SetSphereRadius(5.f, true);

	RightInteractionWidget = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("RightInteractionWidget"));
	RightInteractionWidget->SetupAttachment(RightMoController);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(VRRoot);

	LeftWeaponAmmo = CreateDefaultSubobject<UWidgetComponent>(TEXT("LeftWeaponAmmo"));
	LeftWeaponAmmo->SetupAttachment(LeftMoController);

	RightWeaponAmmo = CreateDefaultSubobject<UWidgetComponent>(TEXT("RightWeaponAmmo"));
	RightWeaponAmmo->SetupAttachment(RightMoController);

	HealthComponent = CreateDefaultSubobject<UHealthComp>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddDynamic(this, &ThisClass::APlayerVRCharacter::OnDie);

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->SetupAttachment(CameraComponent);

}

// Called when the game starts or when spawned
void APlayerVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	LeftWeaponAmmo->SetVisibility(false);
	RightWeaponAmmo->SetVisibility(false);
}

// Called every frame
void APlayerVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("TurnRight", this, &APlayerVRCharacter::RightTurn);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerVRCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerVRCharacter::MoveRight);

	PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &APlayerVRCharacter::GrabRightPressed);
	PlayerInputComponent->BindAction("GrabRight", IE_Released, this, &APlayerVRCharacter::GrabRightReleased);

	PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &APlayerVRCharacter::GrabLeftPressed);
	PlayerInputComponent->BindAction("GrabLeft", IE_Released, this, &APlayerVRCharacter::GrabLeftReleased);

	PlayerInputComponent->BindAction("FireRightWeapon", IE_Pressed, this, &APlayerVRCharacter::FireRightWeapon);
	PlayerInputComponent->BindAction("FireRightWeapon", IE_Released, this, &APlayerVRCharacter::StopFireRightWeapon);
	
	PlayerInputComponent->BindAction("FireLeftWeapon", IE_Pressed, this, &APlayerVRCharacter::FireLeftWeapon);
	PlayerInputComponent->BindAction("FireLeftWeapon", IE_Released, this, &APlayerVRCharacter::StopFireLeftWeapon);
}

float APlayerVRCharacter::GetRightWeaponAmmo() const
{
	if (RightHeldObject)
	{
		if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(RightHeldObject))
		{
			return Weapon->GetCurrentAmmo();
		}
			return 0.f;
	}
	
	return 0.f;
}

float APlayerVRCharacter::GetRightWeaponAmmoInClip() const
{
	if (RightHeldObject)
	{
		if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(RightHeldObject))
		{
			return Weapon->GetCurrentAmmoInClip();
		}
		return 0.f;
	}
	
	return 0.f;
}

float APlayerVRCharacter::GetLeftWeaponAmmo() const
{
	if (LeftHeldObject)
	{
		if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(LeftHeldObject))
		{
			return Weapon->GetCurrentAmmo();
		}
		return 0.f;
	}
	
	return 0.f;
}

float APlayerVRCharacter::GetLeftWeaponAmmoInClip() const
{
	if (LeftHeldObject)
	{
		if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(LeftHeldObject))
		{
			return Weapon->GetCurrentAmmoInClip();
		}
		return 0.f;
	}
	
	return 0.f;
}

float APlayerVRCharacter::GetCurrentHealth() const
{
	return HealthComponent->GetHealth();
}

void APlayerVRCharacter::ApplyDamage(float DamageAmount)
{
	HealthComponent->TakeDamage(DamageAmount);
}

void APlayerVRCharacter::OnDie()
{
	bIsAlive = false;
	
	GrabLeftReleased();
	GrabRightReleased();
	
	GetWorld()->GetFirstPlayerController()->UnPossess();

	GetWorld()->GetTimerManager().SetTimer(RespawnDelayHandle, this, &APlayerVRCharacter::RespawnPlayer, 5.f, false, -1.f);
}

bool APlayerVRCharacter::IsAlive()
{
	return bIsAlive;
}

void APlayerVRCharacter::FireRightWeapon()
{
	if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(RightHeldObject))
	{
		Weapon->Fire();
	}

}

void APlayerVRCharacter::FireLeftWeapon()
{
	if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(LeftHeldObject))
	{
		Weapon->Fire();
	}
}

void APlayerVRCharacter::StopFireRightWeapon()
{
	if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(RightHeldObject))
	{
		Weapon->StopFire();
	}
}

void APlayerVRCharacter::StopFireLeftWeapon()
{
	if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(LeftHeldObject))
	{
		Weapon->StopFire();
	}
}

void APlayerVRCharacter::ResetRotationDelay()
{
	bIsRotate = false;
	GetWorld()->GetTimerManager().ClearTimer(TurnDelayTimerHandle);
}

void APlayerVRCharacter::RespawnPlayer()
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	
	FVector StartLocation = GameMode->FindPlayerStart(GetWorld()->GetFirstPlayerController())->GetActorLocation();
	SetActorLocation(StartLocation);
	HealthComponent->SetMaxHealth();
	GetWorld()->GetFirstPlayerController()->Possess(this);
	bIsAlive = true;
	GetWorld()->GetTimerManager().ClearTimer(RespawnDelayHandle);
}

void APlayerVRCharacter::GrabRightPressed()
{
	TArray<AActor*> OverlappedActors;
	RightCollider->GetOverlappingActors(OverlappedActors);
	if (OverlappedActors.Num() > 0)
	{
		if (ICollectable* Collectable = Cast<ICollectable>(OverlappedActors[0]))
		{
			RightHeldObject = Collectable;
			RightHeldObject->Grip(RightMoController);
			if (Cast<ABaseWeapon>(RightHeldObject))
			{
				RightWeaponAmmo->SetVisibility(true);
			}
		}
	}
}

void APlayerVRCharacter::GrabRightReleased()
{
	if (RightHeldObject)
	{
		if (Cast<ABaseWeapon>(RightHeldObject))
		{
			RightWeaponAmmo->SetVisibility(false);
		}
		RightHeldObject->Drop(RightMoController);
		RightHeldObject = nullptr;
	}
}

void APlayerVRCharacter::GrabLeftPressed()
{
	TArray<AActor*> OverlappedActors;
	LeftCollider->GetOverlappingActors(OverlappedActors);
	if (OverlappedActors.Num() > 0)
	{
		if (ICollectable* Collectable = Cast<ICollectable>(OverlappedActors[0]))
		{
			LeftHeldObject = Collectable;
			LeftHeldObject->Grip(LeftMoController);
			if (Cast<ABaseWeapon>(LeftHeldObject))
			{
				LeftWeaponAmmo->SetVisibility(true);
			}
		}
	}
}

void APlayerVRCharacter::GrabLeftReleased()
{
	if (LeftHeldObject)
	{
		if (Cast<ABaseWeapon>(LeftHeldObject))
		{
			LeftWeaponAmmo->SetVisibility(false);
		}
		LeftHeldObject->Drop(LeftMoController);
		LeftHeldObject = nullptr;
	}
}

void APlayerVRCharacter::RightTurn(float Amount)
{
	if (!bIsRotate)
	{
		if (Amount < -0.5f)
		{
			Amount = -1.f;
		}
		else if (Amount > 0.5f)
		{
			Amount = 1.f;
		}
		else
		{
			Amount = 0.f;
		}

		if (Amount != 0)
		{
			bIsRotate = true;
			AddControllerYawInput(SnapTurnDegrees*Amount);
			GetWorld()->GetTimerManager().SetTimer(TurnDelayTimerHandle, this, &APlayerVRCharacter::ResetRotationDelay, TurnDelayRate, false, -1 );
		}
	}

}

void APlayerVRCharacter::MoveForward(float Amount)
{
	AddMovementInput(GetActorForwardVector(), Amount);
}

void APlayerVRCharacter::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector(), Amount);
}
