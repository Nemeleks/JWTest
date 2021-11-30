// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerVRPawn.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Interfaces/ICollectable.h"
#include "Weapons/BaseWeapon.h"

// Sets default values
APlayerVRPawn::APlayerVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    if (!RootComponent)
    {
	    USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    	RootComponent = SceneComponent;
    }
	
	LeftMoController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMoController"));
	LeftMoController->SetupAttachment(RootComponent);
	LeftMoController->SetTrackingSource(EControllerHand::Left);

	LeftInteractionWidget = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("LeftInteractionWidget"));
	LeftInteractionWidget->SetupAttachment(LeftMoController);

	LeftCollider = CreateDefaultSubobject<USphereComponent>(TEXT("LeftCollider"));
	LeftCollider->SetupAttachment(LeftMoController);
	LeftCollider->SetSphereRadius(50.f, true);

	RightMoController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMoController"));
	RightMoController->SetupAttachment(RootComponent);
	RightMoController->SetTrackingSource(EControllerHand::Right);

	RightCollider = CreateDefaultSubobject<USphereComponent>(TEXT("RightCollider"));
	RightCollider->SetupAttachment(RightMoController);
	RightCollider->SetSphereRadius(50.f, true);

	RightInteractionWidget = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("RightInteractionWidget"));
	RightInteractionWidget->SetupAttachment(RightMoController);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->SetupAttachment(CameraComponent);

}

// Called when the game starts or when spawned
void APlayerVRPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	 auto X = CameraComponent->GetComponentLocation().X - GetActorLocation().X;
	 auto Y = CameraComponent->GetComponentLocation().Y - GetActorLocation().Y;
	 AddActorWorldOffset(FVector(X,Y,0.0f));
	 RootComponent->AddLocalOffset(FVector(-X,-Y,0.0f));
	

	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector NewLocation = CurrentLocation + (ForwardVector*ForwardAxisValue + RightVector*RightAxisValue) * Speed *DeltaTime;

	SetActorLocation(NewLocation, true);


	if (!bIsRotate)
	{
		bIsRotate = true;
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation(0.f, CurrentRotation.Yaw+SnapTurnDegrees*TurnAxisValue, 0.f);
		SetActorRotation(NewRotation);
		GetWorld()->GetTimerManager().SetTimer(TurnDelayTimerHandle, this, &APlayerVRPawn::ResetRotationDelay, TurnDelayRate, false, -1 );
	}
	
}

// Called to bind functionality to input
void APlayerVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("TurnRight", this, &APlayerVRPawn::RightTurn);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerVRPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerVRPawn::MoveRight);

	PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &APlayerVRPawn::GrabRightPressed);
	PlayerInputComponent->BindAction("GrabRight", IE_Released, this, &APlayerVRPawn::GrabRightReleased);

	PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &APlayerVRPawn::GrabLeftPressed);
	PlayerInputComponent->BindAction("GrabLeft", IE_Released, this, &APlayerVRPawn::GrabLeftReleased);

	PlayerInputComponent->BindAction("FireRightWeapon", IE_Pressed, this, &APlayerVRPawn::FireRightWeapon);
	PlayerInputComponent->BindAction("FireLeftWeapon", IE_Pressed, this, &APlayerVRPawn::FireLeftWeapon);
}

void APlayerVRPawn::RightTurn(float Amount)
{
	if (Amount < -0.5)
	{
		TurnAxisValue = -1.f;
	}
	else if(Amount > 0.5)
	{
		TurnAxisValue = 1.f;
	}
	else
	{
		TurnAxisValue = 0.f;
	}
}

void APlayerVRPawn::MoveForward(float Amount)
{
	ForwardAxisValue = Amount;
}

void APlayerVRPawn::MoveRight(float Amount)
{
	RightAxisValue = Amount;
}

void APlayerVRPawn::FireRightWeapon()
{
	if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(RightHeldObject))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("PAWN FIRE"));
		Weapon->Fire();
	}
}

void APlayerVRPawn::FireLeftWeapon()
{
	if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(LeftHeldObject))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("PAWN FIRE"));
		Weapon->Fire();
	}
}

void APlayerVRPawn::ResetRotationDelay()
{
	bIsRotate = false;
}

void APlayerVRPawn::GrabRightPressed()
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

void APlayerVRPawn::GrabRightReleased()
{
if (RightHeldObject)
	{
		RightHeldObject->Drop(RightMoController);
		RightHeldObject = nullptr;
	}
}

void APlayerVRPawn::GrabLeftPressed()
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

void APlayerVRPawn::GrabLeftReleased()
{
	if (LeftHeldObject)
	{
		LeftHeldObject->Drop(LeftMoController);
		LeftHeldObject = nullptr;
	}
}
	
