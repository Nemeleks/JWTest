// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerVRCharacter.generated.h"

UCLASS()
class JOYWAYTEST_API APlayerVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USceneComponent* VRRoot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UMotionControllerComponent* LeftMoController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UMotionControllerComponent* RightMoController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UWidgetInteractionComponent* LeftInteractionWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UWidgetInteractionComponent* RightInteractionWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USphereComponent* LeftCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USphereComponent* RightCollider;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float SnapTurnDegrees = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float Speed = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float TurnDelayRate = 0.5f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void RightTurn(float Amount);
	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void FireRightWeapon();
	void FireLeftWeapon();

	void GrabRightPressed();
	void GrabRightReleased();

	void GrabLeftPressed();
	void GrabLeftReleased();

	float TurnAxisValue = 0.f;
	float ForwardAxisValue = 0.f;
	float RightAxisValue = 0.f;

	FTimerHandle TurnDelayTimerHandle;
	bool bIsRotate = false;

	void ResetRotationDelay();
	
	class IICollectable* RightHeldObject = nullptr;
	class IICollectable* LeftHeldObject = nullptr;
};

