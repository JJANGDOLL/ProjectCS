// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

/**
 * 
 */
class UBlendSpace;
class UStaticMesh;
class UAnimMontage;

UCLASS()
class PROJECTCS_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjCS|Anim", meta = (AllowPrivateAccess = true))
    UBlendSpace* BSStrafe;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjCS|Mesh", meta = (AllowPrivateAccess = true))
    UStaticMesh* WeaponBody;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjCS|Mesh", meta = (AllowPrivateAccess = true))
    FName WeaponSocketName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjCS|Anim", meta = (AllowPrivateAccess = true))
    UAnimMontage* ActionMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjCS|Anim", meta = (AllowPrivateAccess = true))
    UAnimMontage* ChargeMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjCS|Anim", meta = (AllowPrivateAccess = true))
    UAnimMontage* UnchargeMontage;
};
