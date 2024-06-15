// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "ArenaEliminators/Character/ArenaCharacter.h"
#include "ArenaEliminators/GameMode/ArenaGameMode.h"
#include "ArenaEliminators/GameState/ArenaGameState.h"
#include "ArenaEliminators/PlayerState/ArenaPlayerState.h"
#include "ArenaEliminators/HUD/Announcement.h"
#include "ArenaEliminators/HUD/ArenaHUD.h"
#include "ArenaEliminators/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"


void AArenaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ArenaHUD = Cast<AArenaHUD>(GetHUD());

	ServerCheckMatchState();
}

void AArenaPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AArenaPlayerController, MatchState)
}

void AArenaPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (ArenaHUD && ArenaHUD->CharacterOverlay)
		{
			CharacterOverlay = ArenaHUD->CharacterOverlay;
			if (CharacterOverlay)
			{
				SetHUDHealth(HUDHealth, HUDMaxHealth);
				SetHUDScore(HUDScore);
				SetHUDDefeats(HUDDefeats);
			}
		}
	}
}

void AArenaPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetHUDTime();
	//For keep updated server client delta
	CheckTimeSync(DeltaSeconds);
	PollInit();
}

void AArenaPlayerController::ServerCheckMatchState_Implementation()
{
	if (AArenaGameMode* GameMode = Cast<AArenaGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		CooldownTime = GameMode->CooldownTime;
		LevelStartingTime = GameMode->LevelStartTime;
		MatchState = GameMode->GetMatchState();
		ClientJoinMidGame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
}

void AArenaPlayerController::ClientJoinMidGame_Implementation(FName StateOfMatch, float ReceivedWarmupTime, float ReceivedMatchTime, float ReceivedCooldownTime, float ReceivedStartingTime)
{
	WarmupTime = ReceivedWarmupTime;
	MatchTime = ReceivedMatchTime;
	CooldownTime = ReceivedCooldownTime;
	LevelStartingTime = ReceivedStartingTime;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);
	if (ArenaHUD && MatchState == MatchState::WaitingToStart)
	{
		ArenaHUD->AddAnnouncement();
	}
}

void AArenaPlayerController::CheckTimeSync(float DeltaSeconds)
{
	TimeSyncRunningTime += DeltaSeconds;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void AArenaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AArenaCharacter* ArenaCharacter = Cast<AArenaCharacter>(InPawn))
	{
		SetHUDHealth(ArenaCharacter->GetHealth(), ArenaCharacter->GetMaxHealth());
	}
	if (HasAuthority() && IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstLocalPlayerFromController()))
		{
			AArenaCharacter* MyLittleCharacter = Cast<AArenaCharacter>(GetCharacter());
			Subsystem->AddMappingContext(MyLittleCharacter->PlayerMovementContext, 0);
		}
	}
	
}

void AArenaPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void AArenaPlayerController::SetHUDHealth(const float Health, const float MaxHealth)
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (ArenaHUD && ArenaHUD->CharacterOverlay && ArenaHUD->CharacterOverlay->HealthBar && ArenaHUD->CharacterOverlay->HealthText)
	{
		const float HealthPercent = Health / MaxHealth;
		ArenaHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		ArenaHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void AArenaPlayerController::SetHUDScore(const float Score)
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (ArenaHUD && ArenaHUD->CharacterOverlay && ArenaHUD->CharacterOverlay->ScoreAmount)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		ArenaHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HUDScore = Score;
	}
}

void AArenaPlayerController::SetHUDDefeats(const int32 Defeats)
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (ArenaHUD && ArenaHUD->CharacterOverlay && ArenaHUD->CharacterOverlay->DefeatsAmount)
	{
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		ArenaHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HUDDefeats = Defeats;
	}
}

void AArenaPlayerController::SetHUDWeaponAmmo(const int32 Ammo, const int32 MagCap)
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (ArenaHUD && ArenaHUD->CharacterOverlay && ArenaHUD->CharacterOverlay->WeaponAmmoAmount)
	{
		FString AmmoText = FString::Printf(TEXT("%d/%d"), Ammo, MagCap);
		ArenaHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void AArenaPlayerController::SetHUDCarriedAmmo(const int32 Ammo)
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (ArenaHUD && ArenaHUD->CharacterOverlay && ArenaHUD->CharacterOverlay->CarriedAmmoAmount)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		ArenaHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void AArenaPlayerController::SetHUDMatchCountdown(const float Countdown)
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (ArenaHUD && ArenaHUD->CharacterOverlay && ArenaHUD->CharacterOverlay->CountdownText)
	{
		if (Countdown < 0.f)
		{
			ArenaHUD->CharacterOverlay->CountdownText->SetText(FText());
			return;
		}
		int32 Minutes = FMath::FloorToInt(Countdown / 60.f);
		int32 Seconds = Countdown - Minutes * 60.f;
		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		ArenaHUD->CharacterOverlay->CountdownText->SetText(FText::FromString(CountdownText));
	}
}

void AArenaPlayerController::SetHUDAnnouncementCountdown(const float Countdown)
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (ArenaHUD && ArenaHUD->Announcement && ArenaHUD->Announcement->WarmupTime)
	{
		if (Countdown < 0.f)
		{
			ArenaHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}
		int32 Minutes = FMath::FloorToInt(Countdown / 60.f);
		int32 Seconds = Countdown - Minutes * 60.f;
		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		ArenaHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
	}
}

void AArenaPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;
	
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);
	if (CountdownInt != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart)
		{
			UGameplayStatics::PlaySound2D(this, TimerTickSound);
			SetHUDAnnouncementCountdown(SecondsLeft);
			if (SecondsLeft <= 0.f && bCanCountdownEndSound &&  WarmupCountdownEndSound)
			{
				UGameplayStatics::PlaySound2D(this, WarmupCountdownEndSound);
				bCanCountdownEndSound = false;
			}
			bCanCountdownEndSound = true; //bcs Seconds left is 0 at start delay to next frame 
		}
		if (MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(SecondsLeft);
			if (SecondsLeft <= 30.f && TimerTickSound && MatchNearEndSound)
			{
				ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
				if (ArenaHUD && ArenaHUD->CharacterOverlay && ArenaHUD->CharacterOverlay->CountdownText)
				{
					ArenaHUD->CharacterOverlay->CountdownText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
				}
				UGameplayStatics::PlaySound2D(this, TimerTickSound);
				if (bCanEndMusicPlay)
				{
					bCanEndMusicPlay = false;
					MatchEndingMusicComp = UGameplayStatics::SpawnSound2D(this, MatchNearEndSound);
				}
			}
		}
		if (MatchState == MatchState::Cooldown)
		{
			SetHUDAnnouncementCountdown(SecondsLeft);
		}
	}
	CountdownInt = SecondsLeft;
}

void AArenaPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void AArenaPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5 * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

float AArenaPlayerController::GetServerTime()
{
	if (HasAuthority())
	{
		return GetWorld()->GetTimeSeconds();
	}
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void AArenaPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void AArenaPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void AArenaPlayerController::HandleMatchHasStarted()
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (ArenaHUD)
	{
		ArenaHUD->AddCharacterOverlay();
		if (ArenaHUD->Announcement)
		{
			ArenaHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AArenaPlayerController::HandleCooldown()
{
	ArenaHUD = ArenaHUD == nullptr ? Cast<AArenaHUD>(GetHUD()) : ArenaHUD;
	if (ArenaHUD)
	{
		ArenaHUD->CharacterOverlay->RemoveFromParent();
		if (ArenaHUD->Announcement && ArenaHUD->Announcement->AnnouncementText && ArenaHUD->Announcement->InfoText)
		{
			ArenaHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
			FString AnnouncementText("New Match Starts In:");
			ArenaHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));
			
			AArenaGameState* ArenaGameState = Cast<AArenaGameState>(UGameplayStatics::GetGameState(this));
			AArenaPlayerState* ArenaPlayerState = GetPlayerState<AArenaPlayerState>();
			if (ArenaGameState && ArenaPlayerState)
			{
				TArray<AArenaPlayerState*> TopPlayers = ArenaGameState->TopScoringPlayers;
				FString InfoTextString;
				if (TopPlayers.Num() == 0)
				{
					InfoTextString = FString("There is no winner, what a disgrace");
				}
				else if (TopPlayers.Num() == 1 && TopPlayers[0] == ArenaPlayerState)
				{
					InfoTextString = FString("Winner winner chicken dinner!");
				}
				else if (TopPlayers.Num() == 1)
				{
					InfoTextString = FString::Printf(TEXT("Winner: \n%s"), *TopPlayers[0]->GetPlayerName());
				}
				else if (TopPlayers.Num() > 1)
				{
					InfoTextString = FString("Players tied for the win:\n");
					for (auto TiedPlayer : TopPlayers)
					{
						InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
					}
				}
				ArenaHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString));
			}
		}
	}
}
