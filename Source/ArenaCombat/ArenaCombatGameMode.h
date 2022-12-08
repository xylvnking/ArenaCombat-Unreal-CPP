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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		int32 EnemiesAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		int32 EnemiesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		int32 CurrentWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		bool CanWaveSpawn;

	UFUNCTION(BlueprintImplementableEvent)
		void SpawnWave();

	/*UFUNCTION(BlueprintImplementableEvent)
		void SpawnWave2();*/
protected:

	//virtual void BeginPlay() override;


};



