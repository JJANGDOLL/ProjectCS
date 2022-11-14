// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/CharacterBase.h"
#include "EnemyCore.generated.h"

/**
 * 
 */
class UWidgetComponent;
class UEnemyStatus;

UCLASS()
class PROJECTCS_API AEnemyCore : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyCore();


	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjCS|Comp", meta = (AllowPrivateAccess = true))
    UWidgetComponent* StatusWidgetComp;

    UEnemyStatus* StatusWidget;


    void Tick(float DeltaTime) override;

private:
    class UCharacterData* EnemyChracterData;

protected:
	void BeginPlay() override;

private:
    void Attack();
    void SpawnBullet();

    void OnDeadBlendOut(UAnimMontage* AnimMontage, bool bInterrupted);

protected:
    float MaxHealth = 50.f;
    float CurrentHealth = 50.f;

    class ACharacter* PlayerCharacter;
    FTimerHandle AttackHandler;
    FTimerHandle SpawnHandler;
    uint8 AttackCount;

    class UAnimMontage* Montage;
    class UAnimMontage* DeadMontage;
};
