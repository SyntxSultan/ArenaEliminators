// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ArenaEliminators/ArenaTypes/TurningInPlace.h"
#include "ArenaCharacterAnimInstance.generated.h"

class AWeapon;

UCLASS()
class ARENAELIMINATORS_API UArenaCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess = "true"))
	class AArenaCharacter* ArenaCharacter;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	bool bIsInAir;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	bool bWeaponEquipped;
	AWeapon* EquippedWeapon;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	bool bIsCrouched;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	bool bAiming;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	float YawOffset;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	float Lean;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	float AO_Yaw;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	float AO_Pitch;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess = "true"))
	ETurningInPlace TurningInPlace;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FRotator RightHandRotation;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bLocallyControlled;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bRotateRootBone;
};
