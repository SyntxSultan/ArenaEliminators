// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletShell.generated.h"

UCLASS()
class ARENAELIMINATORS_API ABulletShell : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletShell();
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ShellMesh;
	UPROPERTY(EditAnywhere)
	class USoundCue* ShellHitSound;
	UPROPERTY(EditAnywhere)
	float EjectionImpulse;
};
