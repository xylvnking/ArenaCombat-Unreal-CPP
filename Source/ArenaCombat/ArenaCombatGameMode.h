// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaCombatGameMode.generated.h"

UCLASS(minimalapi)
class AArenaCombatGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArenaCombatGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		int32 EnemiesAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		int32 CurrentWave;

protected:
	UFUNCTION(BlueprintCallable)
		void SpawnEnemy();

	//virtual void BeginPlay() override;


};



