// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ArenaGameMode.generated.h"

class AArenaPlayerController;
class AArenaCharacter;

namespace MatchState
{
	extern ARENAELIMINATORS_API const FName Cooldown; // After match end show winner 
}

UCLASS()
class ARENAELIMINATORS_API AArenaGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AArenaGameMode();
	virtual void Tick(float DeltaSeconds) override;
	virtual void PlayerEliminated(AArenaCharacter* EliminatedCharacter, AArenaPlayerController* VictimController, AArenaPlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController);
	
	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;
	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;
	
	float LevelStartTime = 0.f;
protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;
private:
	float CountdownTime = 0.f;
public:
	FORCEINLINE float GetCountdownTime() const { return CooldownTime; }
};
