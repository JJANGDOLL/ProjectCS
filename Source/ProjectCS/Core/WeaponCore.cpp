// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/WeaponCore.h"
#include "../Structs/WeaponData.h"
#include "Utilities/Global.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/BoxComponent.h"

AWeaponCore::AWeaponCore()
{
    Helpers::CreateComponent(this, &WeaponMesh, "Weapon");

    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponMesh->CanCharacterStepUpOn = ECB_No;
    WeaponMesh->SetGenerateOverlapEvents(false);

    SetActorHiddenInGame(true);

    Helpers::GetAsset(&AppearanceCurve, TEXT("CurveFloat'/Game/Assets/Character/AppearanceCurve.AppearanceCurve'"));
}

void AWeaponCore::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AppearanceTL.TickTimeline(DeltaTime);
}

void AWeaponCore::BeginPlay()
{
    Super::BeginPlay();

    AppearanceTLFloat.BindUFunction(this, "SetAppearance");
    AppearanceTLFinish.BindUFunction(this, "EndAppearance");
    AppearanceTL.AddInterpFloat(AppearanceCurve, AppearanceTLFloat);
    AppearanceTL.SetTimelineFinishedFunc(AppearanceTLFinish);
}

void AWeaponCore::Action()
{
    PrintLine();
}

void AWeaponCore::Charge()
{
    bCharged = true;
    ACharacter* character = Cast<ACharacter>(GetOwner());
    if (WeaponData->ChargeMontage && character)
    {
        character->GetMesh()->GetAnimInstance()->Montage_Play(WeaponData->ChargeMontage);
    }
}

void AWeaponCore::Uncharge()
{
    bCharged = false;
    ACharacter* character = Cast<ACharacter>(GetOwner());
    if (WeaponData->UnchargeMontage && character)
    {
        character->GetMesh()->GetAnimInstance()->Montage_Play(WeaponData->UnchargeMontage);
    }
}

UBlendSpace* AWeaponCore::GetBSStrafe()
{
    PrintLine();

    if (!WeaponData)
        return nullptr;

    return WeaponData->BSStrafe;
}
void AWeaponCore::BeEquipped()
{
    PrintLine();

    SetActorHiddenInGame(false);
    AppearanceTL.PlayFromStart();
    ACharacter* character = Cast<ACharacter>(GetOwner());
    if(character)
        AttachToComponent(character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponData->WeaponSocketName);
}

void AWeaponCore::BeUnequipped()
{
    SetActorHiddenInGame(true);
    AppearanceTL.ReverseFromEnd();
}

void AWeaponCore::SetAppearance(float value)
{
}

void AWeaponCore::EndAppearance()
{

}
