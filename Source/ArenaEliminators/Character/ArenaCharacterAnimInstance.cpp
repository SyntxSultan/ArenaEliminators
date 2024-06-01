// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaCharacterAnimInstance.h"
#include "ArenaCharacter.h"
#include "ArenaEliminators/Weapon/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UArenaCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	ArenaCharacter = Cast<AArenaCharacter>(TryGetPawnOwner());
}

void UArenaCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (ArenaCharacter == nullptr)
	{
		ArenaCharacter = Cast<AArenaCharacter>(TryGetPawnOwner());
	}
	if (ArenaCharacter == nullptr) return;
	
	FVector Velocity = ArenaCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = ArenaCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = ArenaCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = ArenaCharacter->IsWeaponEquipped();
	EquippedWeapon = ArenaCharacter->GetEquippedWeapon();
	bIsCrouched = ArenaCharacter->bIsCrouched;
	bAiming = ArenaCharacter->IsAiming();
	bRotateRootBone = ArenaCharacter->ShouldRotateRootBone();
	TurningInPlace = ArenaCharacter->GetTurningInPlace();

	// Offset yaw for strafing
	FRotator AimRotation = ArenaCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ArenaCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 6.f);
	YawOffset = DeltaRotation.Yaw;

	//Lean Calculations
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = ArenaCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	//Aim Offset Calculations
	AO_Yaw = ArenaCharacter->GetAO_Yaw();
	AO_Pitch = ArenaCharacter->GetAO_Pitch();

	//FABRIK IK
	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && ArenaCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), RTS_World);
		FVector OutPos;
		FRotator OutRot;
		ArenaCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPos, OutRot);
		LeftHandTransform.SetLocation(OutPos);
		LeftHandTransform.SetRotation(FQuat(OutRot));
		if (ArenaCharacter->IsLocallyControlled())
		{
			bLocallyControlled = true;
			FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World);
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - ArenaCharacter->GetHitTarget()));
			RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaSeconds, 23.f);
		}
	}
}
