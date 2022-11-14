// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Arrow.h"
#include "../Structs/WeaponData.h"
#include "Utilities/Global.h"
#include "Materials/MaterialInstanceConstant.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "../System/IDamageable.h"
#include "Kismet/GameplayStatics.h"

AArrow::AArrow()
{
    Helpers::CreateComponent(this, &Trail, TEXT("Trail"), WeaponMesh);
    Helpers::GetAsset(&P_Trail, TEXT("ParticleSystem'/Game/Assets/Character/P_Trail.P_Trail'"));
    Helpers::GetAsset(&ImpactParticle, TEXT("ParticleSystem'/Game/ParagonSparrow/FX/Particles/Sparrow/Abilities/Primary/FX/P_Sparrow_Primary_Ballistic_HitPlayer.P_Sparrow_Primary_Ballistic_HitPlayer'"));
    
    Helpers::GetAsset(&WeaponData, TEXT("WeaponData'/Game/Datas/Weapon/ArrowData.ArrowData'"));
    if (!WeaponData)
        return;

    WeaponMesh->SetStaticMesh(WeaponData->WeaponBody);

    Helpers::GetAsset(&AppearanceCurve, TEXT("CurveFloat'/Game/Assets/Character/AppearanceCurveSlow.AppearanceCurveSlow'"));

    Helpers::CreateActorComponent(this, &Projectile, TEXT("Projectile (Movement)")); 
    Projectile->ProjectileGravityScale = 0.f;
    Projectile->Velocity = FVector::ZeroVector;


}

void AArrow::HitCheck_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    PrintLine();

    Projectile->StopMovementImmediately();
    WeaponMesh->SetGenerateOverlapEvents(false);
    Projectile->ProjectileGravityScale = 0.f;

    IIDamageable* damagableActor = Cast<IIDamageable>(OtherActor);

    if (damagableActor)
    {
        float power = 10.f;
        bool critical = false;

        FDamageEvent e;
//         Logger::Log(SweepResult.BoneName.ToString());

        damagableActor->Hitted(power, e, nullptr, this);
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, SweepResult.Location);

        Destroy();
    }
}

void AArrow::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    PrintLine();
}

void AArrow::Action()
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    WeaponMesh->SetCollisionProfileName(TEXT("Bullet"));
    WeaponMesh->SetGenerateOverlapEvents(true);
    WeaponMesh->SetCastShadow(false);

    Trail->BeginTrails(TEXT("Trail1"), TEXT("Trail2"), ETrailWidthMode_FromFirst, 2.5f);

    Projectile->ProjectileGravityScale = 01.f;
    Projectile->Velocity = GetActorRightVector() * 12000.f;

    SetLifeSpan(10.f);
}

void AArrow::BeginPlay()
{
    Super::BeginPlay();

    SetActorHiddenInGame(false);

    UMaterialInstanceConstant* body1 = nullptr;
    UMaterialInstanceConstant* body2 = nullptr;
    UMaterialInstanceConstant* body3 = nullptr;
    Helpers::GetAssetDynamic(&body1, TEXT("MaterialInstanceConstant'/Game/Assets/Character/Mat_Weapon_Black_Inst.Mat_Weapon_Black_Inst'"));
    Helpers::GetAssetDynamic(&body2, TEXT("MaterialInstanceConstant'/Game/Assets/Character/Mat_Weapon_Brown_Inst.Mat_Weapon_Brown_Inst'"));
    Helpers::GetAssetDynamic(&body3, TEXT("MaterialInstanceConstant'/Game/Assets/Character/Mat_Weapon_Black_Inst.Mat_Weapon_Black_Inst'"));

    BodyMaterial1 = UMaterialInstanceDynamic::Create(body1, this);
    WeaponMesh->SetMaterial(0, BodyMaterial1);

    BodyMaterial2 = UMaterialInstanceDynamic::Create(body2, this);
    WeaponMesh->SetMaterial(1, BodyMaterial2);

    BodyMaterial3 = UMaterialInstanceDynamic::Create(body3, this);
    WeaponMesh->SetMaterial(2, BodyMaterial3);

    WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AArrow::HitCheck);
    WeaponMesh->OnComponentHit.AddDynamic(this, &AArrow::OnHit);

    Trail->SetTemplate(P_Trail);
}

void AArrow::SetAppearance(float value)
{
    BodyMaterial1->SetScalarParameterValue("Appearance", value);
    BodyMaterial2->SetScalarParameterValue("Appearance", value);
    BodyMaterial3->SetScalarParameterValue("Appearance", value);
}
