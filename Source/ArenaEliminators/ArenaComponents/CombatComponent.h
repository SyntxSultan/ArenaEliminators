// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaEliminators/HUD/ArenaHUD.h"
#include "ArenaEliminators/Weapon/WeaponTypes.h"
#include "ArenaEliminators/ArenaTypes/CombatState.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 80000.f
class AArenaHUD;
class AArenaPlayerController;
class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENAELIMINATORS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend class AArenaCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void EquipWeapon(AWeapon* WeaponToEquip);
	void Reload();
	UFUNCTION(BlueprintCallable)
	void FinishRelaoding();
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_EquippedWeapon();
	void Fire();

	//Aiming
	void SetAiming(bool bIsAiming);
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);
	//Firing
	void FireButtonPressed(bool bPressed);
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshair(FHitResult& TraceHitResult);
	void SetHUDCrosshairs(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ServerReload();
	void HandleReload();
	int32 AmountToReload();
	void UpdateAmmoValues();
private:	
	AArenaCharacter* Character;
	AArenaPlayerController* PlayerController;
	AArenaHUD* HUD;
	
	FVector HitTarget;
	
	UPROPERTY(ReplicatedUsing=OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;
	
	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	//Crosshair And HUD
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;
	FHUDPackage HUDPackage;
	
	//Fov Settings
	float DefaultFOV;
	float CurrentFOV;
	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 50.f;
	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;
	void InterpFOV(float DeltaTime);
	
	//Auto Fire
	FTimerHandle FireTimer;
	bool bCanFire = true;
	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();
	
	//Carried Ammo for currently equipped weapon
	UPROPERTY(ReplicatedUsing=OnRep_CarriedAmmo)
	int32 CarriedAmmo;
	UFUNCTION()
	void OnRep_CarriedAmmo();
	TMap<EWeaponType, int32> CarriedAmmoMap;
	void InitializeCarriedAmmo();
	
	//Start Ammo values
	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo = 30;

	UPROPERTY(ReplicatedUsing=OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;
	UFUNCTION()
	void OnRep_CombatState();
};
