// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerVRCharacter.h"

#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Weapons/BaseWeapon.h"
#include "Weapons/Pistol.h"

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

}

// Called when the game starts or when spawned
void APlayerVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//
	// if (!bIsRotate)
	// {
	// 	bIsRotate = true;
	// 	FRotator CurrentRotation = GetActorRotation();
	// 	FRotator NewRotation(0.f, CurrentRotation.Yaw+SnapTurnDegrees*TurnAxisValue, 0.f);
	// 	SetActorRotation(NewRotation);
	// 	GetWorld()->GetTimerManager().SetTimer(TurnDelayTimerHandle, this, &APlayerVRCharacter::ResetRotationDelay, TurnDelayRate, false, -1 );
	// }
	
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
	PlayerInputComponent->BindAction("FireLeftWeapon", IE_Pressed, this, &APlayerVRCharacter::FireLeftWeapon);
	
}

void APlayerVRCharacter::FireRightWeapon()
{
	if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(RightHeldObject))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("PAWN FIRE"));
		Weapon->Fire();
	}

}

void APlayerVRCharacter::FireLeftWeapon()
{
	if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(LeftHeldObject))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("PAWN FIRE"));
		Weapon->Fire();
	}
}

void APlayerVRCharacter::ResetRotationDelay()
{
	bIsRotate = false;
	GetWorld()->GetTimerManager().ClearTimer(TurnDelayTimerHandle);
}

void APlayerVRCharacter::GrabRightPressed()
{
	TArray<AActor*> OverlappedActors;
	RightCollider->GetOverlappingActors(OverlappedActors);
	if (OverlappedActors.Num() > 0)
	{
		if (IICollectable* Collectable = Cast<IICollectable>(OverlappedActors[0]))
		{
			RightHeldObject = Collectable;
			RightHeldObject->Grip(RightMoController);
		}
	}
}

void APlayerVRCharacter::GrabRightReleased()
{
	if (RightHeldObject)
	{
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
		if (IICollectable* Collectable = Cast<IICollectable>(OverlappedActors[0]))
		{
			LeftHeldObject = Collectable;
			LeftHeldObject->Grip(LeftMoController);
		}
	}
}

void APlayerVRCharacter::GrabLeftReleased()
{
	if (LeftHeldObject)
	{
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
