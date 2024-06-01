// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaPlayerController.h"

#include "ArenaEliminators/HUD/ArenaHUD.h"
#include "ArenaEliminators/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void AArenaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ArenaHUD = Cast<AArenaHUD>(GetHUD());
}

void AArenaPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	bool bHUDValid = ArenaHUD && ArenaHUD->CharacterOverlay && ArenaHUD->CharacterOverlay->HealthBar && ArenaHUD->CharacterOverlay->HealthText;
	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		ArenaHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		ArenaHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}