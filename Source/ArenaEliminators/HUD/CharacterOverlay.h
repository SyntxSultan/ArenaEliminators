// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class ARENAELIMINATORS_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* HealthText;
};
