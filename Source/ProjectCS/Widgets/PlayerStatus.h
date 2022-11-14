// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatus.generated.h"

class UProgressBar;

UCLASS()
class PROJECTCS_API UPlayerStatus : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void SetHealth(float MaxHealth, float CurHealth);

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UProgressBar* HealthBar;
};
