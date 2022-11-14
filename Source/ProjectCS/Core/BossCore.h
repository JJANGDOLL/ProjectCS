// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/CharacterBase.h"
#include "BossCore.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCS_API ABossCore : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	ABossCore();


	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	void Tick(float DeltaTime) override;

private:
    void Attack();
	void SpawnBullet();

private:
    class UCharacterData* BossCharacter;
	class UAnimMontage* Montage;

    FTimerHandle AttackHandler;
    FTimerHandle SpawnHandler;

	class ACharacter* PlayerCharacter;

protected:
	void BeginPlay() override;

	uint8 BulletCount = 0;

    TSubclassOf<class UEnemyStatus> StatusWidget;
    class UEnemyStatus* WidgetInstance;
    class UUserWidget* EndWidget;

private:
	float MaxHealth = 100.f;
	float CurHealth = 100.f;
};
