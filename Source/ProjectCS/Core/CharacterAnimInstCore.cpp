// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CharacterAnimInstCore.h"
#include "CharacterCore.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "WeaponCore.h"
#include "Utilities/Global.h"

void UCharacterAnimInstCore::NativeBeginPlay()
{
    Super::NativeBeginPlay();

    CharacterCore = Cast<ACharacterCore>(GetOwningActor());
    if (!CharacterCore)
        return;

    CharacterCore->OnWeaponChanged.BindUObject(this, &UCharacterAnimInstCore::WeaponChanged);
}

void UCharacterAnimInstCore::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!CharacterCore)
        return;

    Pitch = CharacterCore->GetController()->GetControlRotation().Pitch;
    Speed = CharacterCore->GetMovementComponent()->Velocity.Size();
    Direction = CalculateDirection(CharacterCore->GetVelocity(), CharacterCore->GetActorRotation());
    bFullBody = GetCurveValue(TEXT("FullBody")) > 0.f ? true : false;
}

void UCharacterAnimInstCore::WeaponChanged()
{
    PrintLine();

    Logger::Log(CharacterCore->GetCurrentWeapon()->GetBSStrafe());

    BSStrafe = CharacterCore->GetCurrentWeapon()->GetBSStrafe();
}
