// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainInterface.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCS_API UMainInterface : public UUserWidget
{
	GENERATED_BODY()
	
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    class UCrosshair* CrosshairWidget;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    class UPlayerStatus* PlayerStatus;

public:
    UPlayerStatus* GetPlayerStatus() { return PlayerStatus; }
};
