// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ArenaGameMode.generated.h"

class AArenaPlayerController;
class AArenaCharacter;

UCLASS()
class ARENAELIMINATORS_API AArenaGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void PlayerEliminated(AArenaCharacter* EliminatedCharacter, AArenaPlayerController* VictimController, AArenaPlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController);
};
