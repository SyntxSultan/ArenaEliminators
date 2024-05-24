// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaCharacterAnimInstance.h"
#include "ArenaCharacter.h"
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
	bIsCrouched = ArenaCharacter->bIsCrouched;
	bAiming = ArenaCharacter->IsAiming();

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
}
