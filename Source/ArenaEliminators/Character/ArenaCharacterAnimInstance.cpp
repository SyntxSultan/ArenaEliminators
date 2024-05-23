// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaCharacterAnimInstance.h"
#include "ArenaCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}
