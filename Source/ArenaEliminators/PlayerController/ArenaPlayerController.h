// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArenaPlayerController.generated.h"

class AArenaGameMode;
class UCharacterOverlay;
class AArenaHUD;
class USoundCue;

UCLASS()
class ARENAELIMINATORS_API AArenaPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void ReceivedPlayer() override; //Sync with server as soon as possible
	virtual float GetServerTime(); //Returns Time sync with server


	void SetHUDHealth(const float Health, const float MaxHealth);
	void SetHUDScore(const float Score);
	void SetHUDDefeats(const int32 Defeats);
	void SetHUDWeaponAmmo(const int32 Ammo, const int32 MagCap);
	void SetHUDCarriedAmmo(const int32 Ammo);
	void SetHUDMatchCountdown(const float Countdown);
	void SetHUDAnnouncementCountdown(const float Countdown);
	void SetHUDTime();
	
	void OnMatchStateSet(FName State);
	void HandleMatchHasStarted();
	void HandleCooldown();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void CheckTimeSync(float DeltaSeconds);
	void PollInit();
	
	/* Sync Time Between Server And Client */
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);
	
	UPROPERTY(EditAnywhere)
	float TimeSyncFrequency = 5.f;
	float TimeSyncRunningTime = 0.f;

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	UFUNCTION(Client, Reliable) //Handling information client match state and timers 
	void ClientJoinMidGame(FName StateOfMatch, float ReceivedWarmupTime, float ReceivedMatchTime, float ReceivedCooldownTime, float ReceivedStartingTime);
private:
	UPROPERTY()
	AArenaHUD* ArenaHUD;
	UPROPERTY()
	AArenaGameMode* ArenaGameMode;
	
	float LevelStartingTime = 0.f;
	float WarmupTime = 0.f;
	float MatchTime = 0.f;
	float CooldownTime = 0.f;
	uint32 CountdownInt = 0;
	
	float ClientServerDelta = 0.f;

	UPROPERTY(ReplicatedUsing=OnRep_MatchState)
	FName MatchState;
	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	UCharacterOverlay* CharacterOverlay;
	bool bInitializeCharacterOverlay = false;
	float HUDHealth;
	float HUDMaxHealth;
	float HUDScore;
	int32 HUDDefeats;
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* TimerTickSound;
	UPROPERTY(EditDefaultsOnly)
	USoundCue* WarmupCountdownEndSound;
	UPROPERTY(EditDefaultsOnly)
	USoundCue* MatchNearEndSound;
	UPROPERTY()
	UAudioComponent* MatchEndingMusicComp;
	bool bCanEndMusicPlay = true;
	bool bCanCountdownEndSound = false;
};
