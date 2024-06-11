// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	
	EWS_Max UMETA(DisplayName = "Max")
};
class AArenaCharacter;
class AArenaPlayerController;
class UAnimationAsset;
class UWidgetComponent;
class ABulletShell;
class UTexture2D;
class USoundCue;

UCLASS()
class ARENAELIMINATORS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Fire(const FVector& HitTarget);
	virtual void OnRep_Owner() override;
	void ShowPickupWidget(bool bShowWidget);
	void SetHUDAmmo();
	void Dropped();
	void AddAmmo(int32 AmmoToAdd);
	//Textures for crosshair
	UPROPERTY(EditAnywhere, Category="Crosshairs")
	UTexture2D* CrosshairCenter;
	UPROPERTY(EditAnywhere, Category="Crosshairs")
	UTexture2D* CrosshairLeft;
	UPROPERTY(EditAnywhere, Category="Crosshairs")
	UTexture2D* CrosshairRight;
	UPROPERTY(EditAnywhere, Category="Crosshairs")
	UTexture2D* CrosshairTop;
	UPROPERTY(EditAnywhere, Category="Crosshairs")
	UTexture2D* CrosshairBottom;
	//Zoom Settings
	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 50.f;
	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;
	// Automatic
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	float FireDelay = 0.15f;
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	bool bAutomatic = true;
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	USoundCue* EquipSound;
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	USoundCue* EmptyFireSound;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	UPROPERTY()
	AArenaCharacter* ArenaOwnerCharacter;
	UPROPERTY()
	AArenaPlayerController* ArenaOwnerController;
	
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(ReplicatedUsing=OnRep_WeaponState, VisibleAnywhere, Category="Weapon Properties")
	EWeaponState WeaponState;
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	EWeaponType WeaponType;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	UWidgetComponent* PickupWidget;
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABulletShell> ShellClass;
	
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_Ammo)
	int32 Ammo;
	UFUNCTION()
	void OnRep_Ammo();
	void SpendRound();
	UPROPERTY(EditAnywhere)
	int32 MagCapacity;
public:
	void SetWeaponState(EWeaponState NewState);
	
	FORCEINLINE USphereComponent*		GetAreaSphere()			const  { return  AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh()			const  { return WeaponMesh; }
	FORCEINLINE float					GetZoomedFOV()          const  { return ZoomedFOV; }
	FORCEINLINE float					GetZoomInterpSpeed()    const  { return ZoomInterpSpeed; }
	FORCEINLINE bool					IsEmpty()			    const  { return Ammo <= 0; }
	FORCEINLINE EWeaponType				GetWeaponType()			const  { return WeaponType; }
	FORCEINLINE int32					GetAmmo()               const  { return Ammo; }
	FORCEINLINE int32					GetMagCapacity()        const  { return MagCapacity; }
};
