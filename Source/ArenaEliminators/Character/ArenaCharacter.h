// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "ArenaCharacter.generated.h"

class UWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class ARENAELIMINATORS_API AArenaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AArenaCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
private:
	//Components
	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadWidget;
	//Inputs
	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* PlayerMovementContext;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* MovementAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* JumpAction;
};
