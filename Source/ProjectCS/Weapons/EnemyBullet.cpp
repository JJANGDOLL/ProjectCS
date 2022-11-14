// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/EnemyBullet.h"
#include "../Structs/WeaponData.h"
#include "Utilities/Global.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "../System/IDamageable.h"

AEnemyBullet::AEnemyBullet()
{
    Helpers::GetAsset(&WeaponData, TEXT("WeaponData'/Game/Datas/Weapon/EnemyBulletData.EnemyBulletData'"));

    if (!WeaponData)
        return;

    WeaponMesh->SetStaticMesh(WeaponData->WeaponBody);
    WeaponMesh->SetWorldScale3D(FVector(7.5f, 7.5f, 7.5f));

    Helpers::CreateActorComponent(this, &Projectile, TEXT("Projectile (Movement)"));
    Projectile->ProjectileGravityScale = 0.f;
    Projectile->Velocity = FVector::ZeroVector;

    Helpers::GetAsset(&ExplodeParticle, TEXT("ParticleSystem'/Game/ParagonIggyScorch/FX/Particles/IggyScorch/Abilities/Primary/FX/P_IggyScorch_Molotov_HitWorld.P_IggyScorch_Molotov_HitWorld'"));
}

void AEnemyBullet::Action()
{
    SetActorHiddenInGame(false);
    WeaponMesh->SetCollisionProfileName(TEXT("EnemyBullet"));
    WeaponMesh->SetGenerateOverlapEvents(true);
    WeaponMesh->SetCastShadow(false);

    Projectile->ProjectileGravityScale = 1.f;
    Projectile->Velocity = GetActorRightVector() * 700.f;
}

void AEnemyBullet::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, SweepResult.Location);

    IIDamageable* damagableActor = Cast<IIDamageable>(OtherActor);

    if (damagableActor)
    {
        FDamageEvent e;
        //         Logger::Log(SweepResult.BoneName.ToString());

        damagableActor->Hitted(15.f, e, nullptr, this);

        Destroy();
    }

    Destroy();
}

void AEnemyBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, Hit.Location);



    Destroy();
}

void AEnemyBullet::BeginPlay()
{
    Super::BeginPlay();

    WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnOverlap);
    WeaponMesh->OnComponentHit.AddDynamic(this, &AEnemyBullet::OnHit);
}
