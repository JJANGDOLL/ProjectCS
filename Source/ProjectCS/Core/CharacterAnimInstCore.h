// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimInstanceBase.h"
#include "CharacterAnimInstCore.generated.h"

class ACharacterCore;
class UBlendSpace;

UCLASS()
class PROJECTCS_API UCharacterAnimInstCore : public UAnimInstanceBase
{
	GENERATED_BODY()
	
private:
	ACharacterCore* CharacterCore;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjCS|Status", meta = (AllowPrivateAccess = true))
    float Direction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjCS|Status", meta = (AllowPrivateAccess = true))
    float Speed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjCS|Status", meta = (AllowPrivateAccess = true))
    float Roll;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjCS|Status", meta = (AllowPrivateAccess = true))
    float Pitch;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjCS|Status", meta = (AllowPrivateAccess = true))
    float Yaw;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjCS|Status", meta = (AllowPrivateAccess = true))
    bool bFullBody;

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjCS|Anim", meta = (AllowPrivateAccess = true))
    UBlendSpace* BSStrafe;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

public:
    void WeaponChanged();
};
