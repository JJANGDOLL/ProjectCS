// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/WeaponCore.h"
#include "Arrow.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCS_API AArrow : public AWeaponCore
{
	GENERATED_BODY()
	
public:
	AArrow();

protected:
    UPROPERTY(VisibleAnywhere)
    class UParticleSystemComponent* Trail;

protected:
    UPROPERTY(VisibleAnywhere)
    class UProjectileMovementComponent* Projectile;

public:
    UFUNCTION(BlueprintNativeEvent, Category = "Collision")
    void HitCheck(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

public:
	void Action() override;

protected:
	void BeginPlay() override;

protected:
	void SetAppearance(float value) override;


private:
    class UMaterialInstanceDynamic* BodyMaterial1;
    class UMaterialInstanceDynamic* BodyMaterial2;
    class UMaterialInstanceDynamic* BodyMaterial3;
    

    class UParticleSystem* P_Trail;

    class UParticleSystem* ImpactParticle;
};
