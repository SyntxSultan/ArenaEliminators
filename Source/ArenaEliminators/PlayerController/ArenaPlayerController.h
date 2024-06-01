// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArenaPlayerController.generated.h"

class AArenaHUD;

UCLASS()
class ARENAELIMINATORS_API AArenaPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetHUDHealth(float Health, float MaxHealth);
protected:
	virtual void BeginPlay() override;
private:
	AArenaHUD* ArenaHUD;
};
