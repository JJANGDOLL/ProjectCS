// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/WeaponCore.h"
#include "Bow.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCS_API ABow : public AWeaponCore
{
	GENERATED_BODY()
	
public:
	ABow();
	void Action() override;
	void Charge() override;
	void Uncharge() override;


	void Tick(float DeltaTime) override;


	void BeUnequipped() override;

protected:
	void OnActionBlendOut(UAnimMontage* AnimMontage, bool bInterrupted);


	void EndAppearance() override;


	void SetAppearance(float value) override;

protected:
	void BeginPlay() override;

private:
    class UMaterialInstanceDynamic* BodyMaterial;
	class AArrow* Arrow;
};
