// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGameMode.h"
#include "ArenaEliminators/Character/ArenaCharacter.h"
#include "ArenaEliminators/GameState/ArenaGameState.h"
#include "ArenaEliminators/PlayerController/ArenaPlayerController.h"
#include "ArenaEliminators/PlayerState/ArenaPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

AArenaGameMode::AArenaGameMode()
{
	bDelayedStart = true;
}
void AArenaGameMode::BeginPlay()
{
	Super::BeginPlay();
	LevelStartTime = GetWorld()->GetTimeSeconds();
}

void AArenaGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartTime;
		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartTime;
		if (CountdownTime <= 0.f)
		{
			RestartGame();
		}
	}
}

void AArenaGameMode::PlayerEliminated(AArenaCharacter* EliminatedCharacter, AArenaPlayerController* VictimController, AArenaPlayerController* AttackerController)
{
	AArenaPlayerState* AttackerPlayerState = AttackerController ? Cast<AArenaPlayerState>(AttackerController->PlayerState) : nullptr;
	AArenaPlayerState* VictimPlayerState = VictimController ? Cast<AArenaPlayerState>(VictimController->PlayerState) : nullptr;
	AArenaGameState* ArenaGameState = GetGameState<AArenaGameState>();
	//Adding attackers score
	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState)
	{
		AttackerPlayerState->AddToScore(1);
		ArenaGameState->UpdateTopScore(AttackerPlayerState);
	}
	// Adding defeats to eliminated player
	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}
	
	if (EliminatedCharacter)
	{
		EliminatedCharacter->Eliminated();
	}
}

void AArenaGameMode::RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController)
{
	if (EliminatedCharacter)
	{
		EliminatedCharacter->Reset();
		EliminatedCharacter->Destroy();
	}
	if (EliminatedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

		TArray<ACharacter*> ExistingCharacters;
		for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
		{
			AController* CurrentController = It->Get();
			ACharacter* CurrentCharacter = CurrentController ? Cast<ACharacter>(CurrentController->GetPawn()) : nullptr;
			if (CurrentCharacter && CurrentCharacter != EliminatedCharacter)
			{
				ExistingCharacters.Add(CurrentCharacter);
			}
		}

		AActor* BestPlayerStart = nullptr;
		float MaxDistance = 0.0f;

		for (AActor* PlayerStart : PlayerStarts)
		{
			float TotalDistance = 0.0f;

			for (ACharacter* Character : ExistingCharacters)
			{
				TotalDistance += FVector::Dist(PlayerStart->GetActorLocation(), Character->GetActorLocation());
			}

			if (TotalDistance > MaxDistance)
			{
				MaxDistance = TotalDistance;
				BestPlayerStart = PlayerStart;
			}
		}

		if (BestPlayerStart)
		{
			RestartPlayerAtPlayerStart(EliminatedController, BestPlayerStart);
		}
		else
		{
			// If no suitable Player Start is found, choose a random one
			int32 Select = FMath::RandRange(0, PlayerStarts.Num() - 1);
			RestartPlayerAtPlayerStart(EliminatedController, PlayerStarts[Select]);
		}
	}
	
	/* only random start
	if (EliminatedCharacter)
	{
		EliminatedCharacter->Reset();
		EliminatedCharacter->Destroy();
	}
	if (EliminatedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Select = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(EliminatedController, PlayerStarts[Select]);
	}
	*/
}

void AArenaGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		AArenaPlayerController* ArenaPlayer = Cast<AArenaPlayerController>(*It);
		if (ArenaPlayer)
		{
			ArenaPlayer->OnMatchStateSet(MatchState);
		}
	}
}
