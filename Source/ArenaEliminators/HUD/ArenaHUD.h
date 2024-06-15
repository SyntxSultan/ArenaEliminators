// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ArenaHUD.generated.h"

class UAnnouncement;
class UCharacterOverlay;
class UTexture2D;
class UUserWidget;

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	UTexture2D* CrosshairCenter;
	UTexture2D* CrosshairLeft;
	UTexture2D* CrosshairRight;
	UTexture2D* CrosshairTop;
	UTexture2D* CrosshairBottom;
	float CrosshairSpread;
	FLinearColor CrosshairColor;
};

UCLASS()
class ARENAELIMINATORS_API AArenaHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;

	bool bDrawCrosshair = true;
	//Character Overlay
	UPROPERTY(EditAnywhere, Category="Player Stats")
	TSubclassOf<UUserWidget> CharacterOverlayClass;
	void AddCharacterOverlay();
	UPROPERTY()
	UCharacterOverlay* CharacterOverlay;
	
	//Announcement Widget
	UPROPERTY(EditAnywhere, Category="Announcements")
	TSubclassOf<UUserWidget> AnnouncementClass;
	void AddAnnouncement();
	UPROPERTY()
	UAnnouncement* Announcement;
protected:
	virtual void BeginPlay() override;
private:
	FHUDPackage HUDPackage;
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);
	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;
public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) {HUDPackage=Package;}
};
