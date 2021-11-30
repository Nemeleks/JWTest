// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable/CollectableBase.h"

#include "Components/BoxComponent.h"

// Sets default values
ACollectableBase::ACollectableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
	    USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    	RootComponent = SceneComponent;
    }

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetSimulatePhysics(true);
	
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(MeshComponent);

	bIsHeld = false;
}

// Called when the game starts or when spawned
void ACollectableBase::BeginPlay()
{
	Super::BeginPlay();

	bIsSimulatePhysics = MeshComponent->IsSimulatingPhysics();
}

// Called every frame
void ACollectableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectableBase::Grip(UMotionControllerComponent* MotionController)
{
	if (!bIsHeld)
	{
		bIsHeld = true;

		CurrentMotionController = MotionController;

		MeshComponent->SetSimulatePhysics(false);
		MeshComponent->AttachToComponent(MotionController, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void ACollectableBase::Drop(UMotionControllerComponent* MotionController)
{
	if ((MotionController == nullptr) ||(CurrentMotionController == MotionController))
	{
		bIsHeld = false;

		MeshComponent->SetSimulatePhysics(true);
		CurrentMotionController = nullptr;
		MeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

