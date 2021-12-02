// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable/CollectableSphere.h"

#include "Components/SphereComponent.h"

ACollectableSphere::ACollectableSphere()
{
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SphereCollider->SetupAttachment(MeshComponent);
}
