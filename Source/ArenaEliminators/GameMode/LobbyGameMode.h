// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARENAELIMINATORS_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	UFUNCTION(BlueprintCallable)
	void StartGameImmidiately();

	UPROPERTY(EditDefaultsOnly)
	int32 PlayersToStart = 1;
};
