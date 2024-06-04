// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaPlayerController.h"

#include "ArenaEliminators/Character/ArenaCharacter.h"
#include "ArenaEliminators/HUD/ArenaHUD.h"
#include "ArenaEliminators/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void AArenaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ArenaHUD = Cast<AArenaHUD>(GetHUD());
}

void AArenaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AArenaCharacter* ArenaCharacter = Cast<AArenaCharacter>(InPawn))
	{
		SetHUDHealth(ArenaCharacter->GetHealth(), ArenaCharacter->GetMaxHealth());
	}
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

void AArenaPlayerController::SetHUDScore(float Score)
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (bool bHUDValid = ArenaHUD && ArenaHUD->CharacterOverlay && ArenaHUD->CharacterOverlay->ScoreAmount)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		ArenaHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
}

void AArenaPlayerController::SetHUDDefeats(int32 Defeats)
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (bool bHUDValid = ArenaHUD && ArenaHUD->CharacterOverlay && ArenaHUD->CharacterOverlay->DefeatsAmount)
	{
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		ArenaHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
}

void AArenaPlayerController::SetHUDWeaponAmmo(int32 Ammo, int32 MagCap)
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (bool bHUDValid = ArenaHUD && ArenaHUD->CharacterOverlay && ArenaHUD->CharacterOverlay->WeaponAmmoAmount)
	{
		FString AmmoText = FString::Printf(TEXT("%d/%d"), Ammo, MagCap);
		ArenaHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void AArenaPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (bool bHUDValid = ArenaHUD && ArenaHUD->CharacterOverlay && ArenaHUD->CharacterOverlay->CarriedAmmoAmount)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		ArenaHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}