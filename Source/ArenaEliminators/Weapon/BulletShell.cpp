// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletShell.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ABulletShell::ABulletShell()
{
	PrimaryActorTick.bCanEverTick = false;
	ShellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(ShellMesh);
	ShellMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	ShellMesh->SetSimulatePhysics(true);
	ShellMesh->SetEnableGravity(true);
	ShellMesh->SetNotifyRigidBodyCollision(true);
	EjectionImpulse = 9.f;	
}
void ABulletShell::BeginPlay()
{
	Super::BeginPlay();
	ShellMesh->AddImpulse(GetActorForwardVector() * EjectionImpulse);
	ShellMesh->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	SetLifeSpan(5.f);
}
void ABulletShell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,FVector NormalImpulse, const FHitResult& Hit)
{
	if (!ShellHitSound) return;
	UGameplayStatics::PlaySoundAtLocation(this, ShellHitSound, GetActorLocation());
	ShellMesh->SetNotifyRigidBodyCollision(false);
}



