// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/EnemyBullet2.h"
#include "../Structs/WeaponData.h"
#include "Utilities/Global.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "../System/IDamageable.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

AEnemyBullet2::AEnemyBullet2()
{
    Helpers::GetAsset(&WeaponData, TEXT("WeaponData'/Game/Datas/Weapon/EnemyBulletData2.EnemyBulletData2'"));

    if (!WeaponData)
        return;

    WeaponMesh->SetStaticMesh(WeaponData->WeaponBody);
    WeaponMesh->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));

    Helpers::CreateActorComponent(this, &Projectile, TEXT("Projectile (Movement)"));
    Projectile->ProjectileGravityScale = 0.f;
    Projectile->Velocity = FVector::ZeroVector;

    Helpers::GetAsset(&ExplodeParticle, TEXT("ParticleSystem'/Game/ParagonIggyScorch/FX/Particles/IggyScorch/Abilities/Primary/FX/P_IggyScorch_Molotov_HitWorld.P_IggyScorch_Molotov_HitWorld'"));
}

void AEnemyBullet2::Action()
{
    SetActorHiddenInGame(false);
    WeaponMesh->SetCollisionProfileName(TEXT("EnemyBullet"));
    WeaponMesh->SetGenerateOverlapEvents(true);
    WeaponMesh->SetCastShadow(false);

    SetLifeSpan(10.f);

    Projectile->Velocity = GetActorForwardVector() * 300.f;
}

void AEnemyBullet2::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
//     UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, SweepResult.Location);

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

void AEnemyBullet2::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
//     UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, Hit.Location);

    Destroy();
}

void AEnemyBullet2::BeginPlay()
{
    Super::BeginPlay();

    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if(PlayerCharacter)
        SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerCharacter->GetActorLocation()));

    WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet2::OnOverlap);
    WeaponMesh->OnComponentHit.AddDynamic(this, &AEnemyBullet2::OnHit);
}
