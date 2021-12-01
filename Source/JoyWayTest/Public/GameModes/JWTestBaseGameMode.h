// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JWTestBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JOYWAYTEST_API AJWTestBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Respawn")
	float RespawnDelay = 3.f;

	FTimerHandle RespawnTimerHandle;
	
public:

	virtual void BeginPlay() override;

	void Respawn(AController* Controller);
	
protected:
	UFUNCTION()
	void Spawn(AController* Controller);

	
};
