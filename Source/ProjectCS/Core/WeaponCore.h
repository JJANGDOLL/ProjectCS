// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ActorBase.h"
#include "Components/TimelineComponent.h"
#include "WeaponCore.generated.h"

/**
 * 
 */
class UBlendSpace;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class PROJECTCS_API AWeaponCore : public AActorBase
{
	GENERATED_BODY()

public:
	AWeaponCore();


    void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* WeaponMesh;
	

protected:
    void BeginPlay() override;

public:
	virtual void Action();
	virtual void Charge();
	virtual void Uncharge();

public:
 	UBlendSpace* GetBSStrafe();
	virtual void BeEquipped();
    virtual void BeUnequipped();

protected:
    class UWeaponData* WeaponData;
	bool bCharged = false;

protected:
    class UCurveFloat* AppearanceCurve;
private:
    FTimeline AppearanceTL;
    FOnTimelineFloat AppearanceTLFloat;
    FOnTimelineEvent AppearanceTLFinish;

protected:
    UFUNCTION()
    virtual void SetAppearance(float value);

    UFUNCTION()
    virtual void EndAppearance();
};
