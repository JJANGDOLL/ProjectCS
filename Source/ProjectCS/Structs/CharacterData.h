// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCS_API UCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjCS|Mesh", meta = (AllowPrivateAccess = true))
    class USkeletalMesh* CharacterBody;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjCS|Anim", meta = (AllowPrivateAccess = true))
    TSubclassOf<class UAnimInstance> CharacterAnimInst;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjCS|Anim", meta = (AllowPrivateAccess = true))
    class UAnimMontage* TestMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjCS|Anim", meta = (AllowPrivateAccess = true))
    class UAnimMontage* TestMontage2;
};
