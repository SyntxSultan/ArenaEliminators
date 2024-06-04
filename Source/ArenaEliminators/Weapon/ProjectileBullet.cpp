// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBullet.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (AController* Controller = OwnerCharacter->GetController())
		{			
			UGameplayStatics::ApplyDamage(OtherActor, Damage, Controller, this, UDamageType::StaticClass());
		}
	}
	//Projectile will be destroyed after super
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
