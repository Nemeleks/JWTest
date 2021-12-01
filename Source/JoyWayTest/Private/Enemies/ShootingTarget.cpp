// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/ShootingTarget.h"

#include "Components/BoxComponent.h"
#include "Components/HealthComp.h"

// Sets default values
AShootingTarget::AShootingTarget()
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

	HealthComp = CreateDefaultSubobject<UHealthComp>(TEXT("HealthComponent"));
	HealthComp->OnDie.AddDynamic(this, &AShootingTarget::OnDie);
}

// Called when the game starts or when spawned
void AShootingTarget::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShootingTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShootingTarget::ApplyDamage(float DamageAmount)
{
	HealthComp->TakeDamage(DamageAmount);
}

void AShootingTarget::OnDie()
{
	Destroy();
}
