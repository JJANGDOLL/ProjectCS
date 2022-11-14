// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/WeaponCore.h"
#include "Katana.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCS_API AKatana : public AWeaponCore
{
	GENERATED_BODY()
	
public:
	AKatana();

	virtual void Action() override;

	void Tick(float DeltaTime) override;


	void Uncharge() override;

private:
    void OnActionBlendOut(UAnimMontage* AnimMontage, bool bInterrupted);

protected:
	void BeginPlay() override;

protected:
	void EndAppearance() override;


	void SetAppearance(float value) override;

    UFUNCTION(BlueprintNativeEvent, Category = "Collision")
    void HitCheck(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
    class UMaterialInstanceDynamic* BodyMaterial1;
    class UMaterialInstanceDynamic* BodyMaterial2;
    class UMaterialInstanceDynamic* BodyMaterial3;
    class UMaterialInstanceDynamic* BodyMaterial4;
};
