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
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo, int32 MagCap);
	void SetHUDCarriedAmmo(int32 Ammo);
private:
	UPROPERTY()
	AArenaHUD* ArenaHUD;
};
