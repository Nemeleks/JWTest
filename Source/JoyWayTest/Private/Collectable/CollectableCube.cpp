// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable/CollectableCube.h"

#include "Components/BoxComponent.h"

ACollectableCube::ACollectableCube()
{
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxCollider->SetupAttachment(MeshComponent);
}