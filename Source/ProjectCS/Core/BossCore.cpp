// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BossCore.h"
#include "Utilities/Global.h"
#include "Components/CapsuleComponent.h"
#include "../Structs/CharacterData.h"
#include "Kismet/GameplayStatics.h"
#include "../Weapons/EnemyBullet2.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "../Widgets/EnemyStatus.h"
#include "CharacterCore.h"

ABossCore::ABossCore()
{
    Helpers::GetAsset(&BossCharacter, TEXT("CharacterData'/Game/Datas/Character/BossCharacter.BossCharacter'"));

    if (!BossCharacter)
        return;

    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

    GetMesh()->SetSkeletalMesh(BossCharacter->CharacterBody);
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
    GetMesh()->SetGenerateOverlapEvents(true);
    GetMesh()->SetCollisionProfileName(TEXT("Body"));
    GetMesh()->SetAnimInstanceClass(BossCharacter->CharacterAnimInst);

    GetCapsuleComponent()->SetWorldScale3D(FVector(2.f, 2.f, 2.f));

    Helpers::GetAsset(&Montage, TEXT("AnimMontage'/Game/Assets/Character/AM_Primary_Fire.AM_Primary_Fire'"));
    Helpers::GetClass(&StatusWidget, TEXT("WidgetBlueprint'/Game/Assets/Character/BossStatus.BossStatus_C'"));
}

float ABossCore::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

    CurHealth = FMath::Clamp<float>(CurHealth - damage, 0, MaxHealth);
    if(WidgetInstance)
        WidgetInstance->SetHealth(MaxHealth, CurHealth);

    return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void ABossCore::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CurHealth <= 0)
    {
        SetActorTickEnabled(false);

        WidgetInstance->RemoveFromParent();
        Destroy();
    }
}

void ABossCore::Attack()
{
    ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    BulletCount = 10;

    GetWorldTimerManager().UnPauseTimer(SpawnHandler);
}

void ABossCore::SpawnBullet()
{
//     if (!PlayerCharacter)
//         return;

    if (BulletCount == 0)
    {
        GetWorldTimerManager().PauseTimer(SpawnHandler);
        return;
    }

    GetMesh()->GetAnimInstance()->Montage_Play(Montage);

    FTransform bulletTransform = GetMesh()->GetSocketTransform(TEXT("BulletFront"), RTS_World);

    AEnemyBullet2* bullet = GetWorld()->SpawnActor<AEnemyBullet2>(AEnemyBullet2::StaticClass(), bulletTransform);
    if (bullet)
    {
        bullet->Action();
        BulletCount--;
    }
}

void ABossCore::BeginPlay()
{
    Super::BeginPlay();

    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    ACharacterCore* characterCore = Cast<ACharacterCore>(PlayerCharacter);

//     StatusWidget->AddToViewport();
    if (characterCore)
    {
        WidgetInstance = CreateWidget<UEnemyStatus>(UGameplayStatics::GetPlayerController(GetWorld(), 0), StatusWidget, TEXT("Status"));
        WidgetInstance->AddToViewport();
        WidgetInstance->SetHealth(MaxHealth, CurHealth);
    }

    GetWorldTimerManager().SetTimer(AttackHandler, this, &ABossCore::Attack, 5.f, true, 0.f);
    GetWorldTimerManager().SetTimer(SpawnHandler, this, &ABossCore::SpawnBullet, 0.3f, true, 0.f);
    GetWorldTimerManager().PauseTimer(SpawnHandler);
    
}
