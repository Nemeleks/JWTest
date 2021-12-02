// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/JWTestBaseGameMode.h"


void AJWTestBaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJWTestBaseGameMode::Respawn(AController* Controller)
{
	FTimerDelegate RespawnDelegate;
	RespawnDelegate.BindUFunction(this, FName("Spawn"), Controller);
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);
}

void AJWTestBaseGameMode::Spawn(AController* Controller)
{
	FVector PlayerStartLocation = FindPlayerStart(Controller)->GetActorLocation();
	FRotator PlayerStartRotation = FindPlayerStart(Controller)->GetActorRotation();
	if (APawn* Pawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, PlayerStartLocation, PlayerStartRotation))
	{
		Controller->Possess(Pawn);
	}

}


