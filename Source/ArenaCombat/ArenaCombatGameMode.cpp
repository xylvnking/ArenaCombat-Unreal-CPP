// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArenaCombatGameMode.h"
#include "ArenaCombatCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArenaCombatGameMode::AArenaCombatGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

//void AArenaCombatGameMode::SpawnWave()
//{
//	SpawnWave2();
//}

//void AArenaCombatGameMode::BeginPlay()
//{
//	Super::BeginPlay();
//	UE_LOG(LogTemp, Log, TEXT("YEAAAH"));
//
//}
