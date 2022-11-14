// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/WeaponCore.h"
#include "EnemyBullet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCS_API AEnemyBullet : public AWeaponCore
{
	GENERATED_BODY()
	
public:
	AEnemyBullet();


	void Action() override;


	UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
    UPROPERTY(VisibleAnywhere)
    class UProjectileMovementComponent* Projectile;

	void BeginPlay() override;

	class UParticleSystem* ExplodeParticle;
};
