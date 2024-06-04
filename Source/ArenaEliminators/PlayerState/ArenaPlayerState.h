// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ArenaPlayerState.generated.h"

class AArenaCharacter;
class AArenaPlayerController;

UCLASS()
class ARENAELIMINATORS_API AArenaPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Score() override;
	UFUNCTION()
	virtual void OnRep_Defeats();
	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatAmount);
private:
	AArenaCharacter* Character;
	AArenaPlayerController* Controller;
	UPROPERTY(ReplicatedUsing=OnRep_Defeats)
	int32 Defeats;
};
