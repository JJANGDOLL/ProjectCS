// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Bow.h"
#include "../Structs/WeaponData.h"
#include "Utilities/Global.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Arrow.h"

ABow::ABow()
{
    Helpers::GetAsset(&WeaponData, TEXT("WeaponData'/Game/Datas/Weapon/BowData.BowData'"));

    if (!WeaponData)
        return;

    WeaponMesh->SetStaticMesh(WeaponData->WeaponBody);
}

void ABow::Action()
{
    PrintLine();

    ACharacter* character = Cast<ACharacter>(GetOwner());
    if (WeaponData->ActionMontage)
    {
        FOnMontageEnded BlendOutDele;
        BlendOutDele.BindUObject(this, &ABow::OnActionBlendOut);
        character->GetMesh()->GetAnimInstance()->Montage_Play(WeaponData->ActionMontage);
        if (Arrow)
        {
            Arrow->Action();
            Arrow = nullptr;
        }
        character->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendOutDele, WeaponData->ActionMontage);
    }
}

void ABow::Charge()
{
    Super::Charge();

    Arrow = GetWorld()->SpawnActor<AArrow>(AArrow::StaticClass(), FTransform());
    if (Arrow)
    {
        Arrow->SetOwner(GetOwner());
        Arrow->BeEquipped();
    }
}

void ABow::Uncharge()
{
    Super::Uncharge();

    if (Arrow)
        Arrow->Destroy();
}

void ABow::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABow::BeUnequipped()
{
    Super::BeUnequipped();

    if (Arrow)
        Arrow->Destroy();
}

void ABow::OnActionBlendOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
    PrintLine();

    if (bCharged)
    {
        Charge();
    }
}

void ABow::EndAppearance()
{
}

void ABow::SetAppearance(float value)
{
    BodyMaterial->SetScalarParameterValue("Appearance", value);
}

void ABow::BeginPlay()
{
    Super::BeginPlay();

    UMaterialInstanceConstant* body = nullptr;
    Helpers::GetAssetDynamic(&body, TEXT("MaterialInstanceConstant'/Game/Assets/Character/Mat_Weapon_Brown_Inst.Mat_Weapon_Brown_Inst'"));

    BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
    WeaponMesh->SetMaterial(0, BodyMaterial);
}
