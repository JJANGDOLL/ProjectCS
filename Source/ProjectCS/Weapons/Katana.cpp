// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Katana.h"
#include "../Structs/WeaponData.h"
#include "Utilities/Global.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInstanceConstant.h"
#include "../System/IDamageable.h"

AKatana::AKatana()
{
    Helpers::GetAsset(&WeaponData, TEXT("WeaponData'/Game/Datas/Weapon/KatanaData.KatanaData'"));

    if (!WeaponData)
        return;

    WeaponMesh->SetStaticMesh(WeaponData->WeaponBody);
}

void AKatana::Action()
{
    PrintLine();
    ACharacter* character = Cast<ACharacter>(GetOwner());
    if (WeaponData->ActionMontage)
    {
        WeaponMesh->SetGenerateOverlapEvents(true);

        FOnMontageEnded BlendOutDele;
        BlendOutDele.BindUObject(this, &AKatana::OnActionBlendOut);
        character->GetMesh()->GetAnimInstance()->Montage_Play(WeaponData->ActionMontage);
        character->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendOutDele, WeaponData->ActionMontage);
    }
}

void AKatana::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AKatana::Uncharge()
{
    Super::Uncharge();
}

void AKatana::OnActionBlendOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
    WeaponMesh->SetGenerateOverlapEvents(false);

    if (bCharged)
    {
        Charge();
    }
}

void AKatana::BeginPlay()
{
    Super::BeginPlay();

    UMaterialInstanceConstant* body1 = nullptr;
    UMaterialInstanceConstant* body2 = nullptr;
    UMaterialInstanceConstant* body3 = nullptr;
    UMaterialInstanceConstant* body4 = nullptr;
    Helpers::GetAssetDynamic(&body1, TEXT("MaterialInstanceConstant'/Game/Assets/Character/Mat_Weapon_DirtWhite_Inst.Mat_Weapon_DirtWhite_Inst'"));
    Helpers::GetAssetDynamic(&body2, TEXT("MaterialInstanceConstant'/Game/Assets/Character/Mat_Weapon_Black_Inst.Mat_Weapon_Black_Inst'"));
    Helpers::GetAssetDynamic(&body3, TEXT("MaterialInstanceConstant'/Game/Assets/Character/Mat_Weapon_White_Inst.Mat_Weapon_White_Inst'"));
    Helpers::GetAssetDynamic(&body4, TEXT("MaterialInstanceConstant'/Game/Assets/Character/Mat_Weapon_Red_Inst.Mat_Weapon_Red_Inst'"));

    BodyMaterial1 = UMaterialInstanceDynamic::Create(body1, this);
    WeaponMesh->SetMaterial(0, BodyMaterial1);

    BodyMaterial2 = UMaterialInstanceDynamic::Create(body2, this);
    WeaponMesh->SetMaterial(1, BodyMaterial2);

    BodyMaterial3 = UMaterialInstanceDynamic::Create(body3, this);
    WeaponMesh->SetMaterial(2, BodyMaterial3);

    BodyMaterial4 = UMaterialInstanceDynamic::Create(body4, this);
    WeaponMesh->SetMaterial(3, BodyMaterial4);

    WeaponMesh->SetCollisionProfileName(TEXT("Bullet"));
    WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AKatana::HitCheck);
}

void AKatana::EndAppearance()
{
}

void AKatana::SetAppearance(float value)
{
    BodyMaterial1->SetScalarParameterValue("Appearance", value);
    BodyMaterial2->SetScalarParameterValue("Appearance", value);
    BodyMaterial3->SetScalarParameterValue("Appearance", value);
    BodyMaterial4->SetScalarParameterValue("Appearance", value);
}

void AKatana::HitCheck_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    PrintLine();

    IIDamageable* damagableActor = Cast<IIDamageable>(OtherActor);

    if (damagableActor)
    {
        bool critical = false;

        FDamageEvent e;

        ACharacter* character = Cast<ACharacter>(GetOwner());

        damagableActor->Hitted(character->GetMesh()->GetAnimInstance()->GetCurveValue(TEXT("Damage")), e, nullptr, this);
    }
}
