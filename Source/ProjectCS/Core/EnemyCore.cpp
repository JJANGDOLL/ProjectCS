// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/EnemyCore.h"
#include "Utilities/Global.h"
#include "../Structs/CharacterData.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "../Widgets/EnemyStatus.h"
#include "Kismet/GameplayStatics.h"
#include "../Weapons/EnemyBullet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"

AEnemyCore::AEnemyCore()
{
    Helpers::CreateComponent(this, &StatusWidgetComp, TEXT("StatusWidget"), GetCapsuleComponent());

    Helpers::GetAsset(&EnemyChracterData, TEXT("CharacterData'/Game/Datas/Character/EnemyCharacter.EnemyCharacter'"));

    if (!EnemyChracterData)
        return;

    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

    GetMesh()->SetSkeletalMesh(EnemyChracterData->CharacterBody);
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
    GetMesh()->SetGenerateOverlapEvents(true);
    GetMesh()->SetCollisionProfileName(TEXT("Body"));
    GetMesh()->SetAnimInstanceClass(EnemyChracterData->CharacterAnimInst);

    TSubclassOf<UUserWidget> statusWidgetClass;
    Helpers::GetClass(&statusWidgetClass, TEXT("WidgetBlueprint'/Game/Assets/Character/EnemyStatus.EnemyStatus_C'"));

    StatusWidgetComp->SetWidgetClass(statusWidgetClass);
    StatusWidgetComp->SetDrawSize(FVector2D(100.f, 20.f));
    StatusWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 15.f));
    StatusWidgetComp->SetWidgetSpace(EWidgetSpace::World);
    StatusWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Helpers::GetAsset(&Montage, TEXT("AnimMontage'/Game/Assets/Character/AM_Scorch_Primary_Fire_Med.AM_Scorch_Primary_Fire_Med'"));
    Helpers::GetAsset(&DeadMontage, TEXT("AnimMontage'/Game/Assets/Character/AM_Death_Back.AM_Death_Back'"));
}

float AEnemyCore::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    CurrentHealth = FMath::Clamp<float>(CurrentHealth - damage, 0, MaxHealth);
    StatusWidget->SetHealth(MaxHealth, CurrentHealth);

    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemyCore::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerCharacter)
    {
        FRotator widgetRot = PlayerCharacter->GetCapsuleComponent()->GetForwardVector().Rotation() * -1;
        FVector dirChar = GetActorLocation() - PlayerCharacter->GetActorLocation();

        dirChar.Normalize();
        StatusWidgetComp->SetVisibility(true);
        StatusWidgetComp->SetRelativeRotation(FRotator(0.f, 180.f, 0.f) - GetActorRotation());
        StatusWidgetComp->AddRelativeRotation(FRotator(0.f, widgetRot.Yaw * -1.f, 0.f));

        SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerCharacter->GetActorLocation()), DeltaTime, 1.f));
    }

    if (CurrentHealth <= 0)
    {
        SetActorTickEnabled(false);

        FOnMontageEnded BlendOutDele;
        BlendOutDele.BindUObject(this, &AEnemyCore::OnDeadBlendOut);
        GetMesh()->GetAnimInstance()->Montage_Play(DeadMontage);
        GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendOutDele, DeadMontage);
    }
}

void AEnemyCore::BeginPlay()
{
    Super::BeginPlay();

    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    StatusWidget = Cast<UEnemyStatus>(StatusWidgetComp->GetWidget());

    GetWorldTimerManager().SetTimer(AttackHandler, this, &AEnemyCore::Attack, 3.f, true, 0.f);
}

void AEnemyCore::Attack()
{
    ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    FTransform bulletTransform = GetMesh()->GetSocketTransform(TEXT("BulletFront"), RTS_World);

    GetMesh()->GetAnimInstance()->Montage_Play(Montage);

    AEnemyBullet* bullet = GetWorld()->SpawnActor<AEnemyBullet>(AEnemyBullet::StaticClass(), bulletTransform);
    if (bullet)
    {
        bullet->Action();
    }
}

void AEnemyCore::OnDeadBlendOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
    Destroy();
}

