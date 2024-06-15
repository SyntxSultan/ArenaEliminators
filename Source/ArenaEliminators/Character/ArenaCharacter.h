// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "ArenaEliminators/ArenaComponents/CombatComponent.h"
#include "ArenaEliminators/ArenaTypes/CombatState.h"
#include "ArenaEliminators/ArenaTypes/TurningInPlace.h"
#include "ArenaEliminators/Interfaces/InteractWithCrosshairsInterface.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "ArenaCharacter.generated.h"

class AWeapon;
class UCombatComponent;
class AArenaPlayerState;
class AArenaPlayerController;
class UForceFeedbackEffect;
class UWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UCameraShakeBase;
class UInputAction;
class UAnimMontage;
class USoundCue;

UCLASS()
class ARENAELIMINATORS_API AArenaCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	AArenaCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	virtual void Destroyed() override;
	virtual void OnRep_ReplicatedMovement() override;
	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();
	void PlayEliminationMontage();
	void Eliminated();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastEliminated();
	
	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* PlayerMovementContext;
protected:
	virtual void BeginPlay() override;
	void RotateInPlace(float DeltaTime);
	virtual void Jump() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed(const FInputActionValue& Value);
	void CrouchButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void SprintButtonPressed();
	void SprintButtonReleased();
	void ReloadButtonPressed();
	void CalculateAO_Pitch();
	void AimOffset(float DeltaTime);
	void SimProxiesTurn();
	void FireButtonPressed();
	void FireButtonReleased();
	void PlayHitReactMontage();
	void UpdateHealthHUD();
	void PollInit();
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
private:
	bool bSprinting = false;
	UPROPERTY()
	AArenaPlayerController* ArenaPlayerController;
	UPROPERTY()
	AArenaPlayerState* ArenaPlayerState;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> WalkCameraShake;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> SprintCameraShake;

	UPROPERTY(EditAnywhere)
	UForceFeedbackEffect* HitForceFeedback;
	UPROPERTY(EditAnywhere)
	UForceFeedbackEffect* DieForceFeedback;
	
	//Components
	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera, meta=(AllowPrivateAccess="true"))
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCombatComponent* Combat;
	
	UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);
	
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);
	bool bRotateRootBone;
	float TurnThreshold = 25.f;
	
	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;
	void HideCameraIfCharacterClose();
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();
	
	//Player health
	UPROPERTY(EditAnywhere, Category="Player Stats")
	float MaxHealth = 100.f;
	UPROPERTY(ReplicatedUsing=OnRep_Health, VisibleAnywhere, Category="Player Stats")
	float Health = 100.f;
	UFUNCTION()
	void OnRep_Health();

	bool bEliminated = false;
	FTimerHandle ElimTimer;
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;
	void ElimTimerFinished();

	/* Dissolve Effect */
	
	// Material instance set on the Blueprint, used with the dynamic material instance
	UPROPERTY(EditAnywhere, Category=Eliminate)
	UMaterialInstance* DissolveMaterialInstance;
	// Dynamic instance that we can change at runtime
	UPROPERTY(VisibleAnywhere, Category=Eliminate)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;
	FOnTimelineFloat DissolveTrack;
	UPROPERTY(EditDefaultsOnly, Category=Eliminate)
	UCurveFloat* DissolveCurve;
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();

	/* Clean Bot */
	UPROPERTY(EditAnywhere, Category=Eliminate)
	UParticleSystem* CleanBotEffect;
	UPROPERTY(VisibleAnywhere, Category=Eliminate)
	UParticleSystemComponent* CleanBotComponent;
	UPROPERTY(EditAnywhere, Category=Eliminate)
	USoundCue* CleanBotCleanSound;
	
	/* For aim offset calculations */
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	
	//Animation Montages
	UPROPERTY(EditAnywhere, Category=Combat)
	UAnimMontage* FireWeaponMontage;
	UPROPERTY(EditAnywhere, Category=Combat)
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditAnywhere, Category=Combat)
	UAnimMontage* EliminationMontage;
	UPROPERTY(EditAnywhere, Category=Combat)
	UAnimMontage* ReloadMontage;
	
	//Inputs
	
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* MovementAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* EKeyAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* ReloadAction;
public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	AWeapon* GetEquippedWeapon();
	FVector GetHitTarget() const;
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE bool IsEliminated() const { return bEliminated; }
	FORCEINLINE bool IsSprinting() const { return bSprinting; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE ECombatState GetCombatState() const { return Combat == nullptr ? ECombatState::ECS_MAX : Combat->CombatState; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
};
