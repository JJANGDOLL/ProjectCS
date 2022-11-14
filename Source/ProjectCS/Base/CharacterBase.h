// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../System/IDamageable.h"
#include "CharacterBase.generated.h"

UCLASS()
class PROJECTCS_API ACharacterBase : public ACharacter, public IIDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();


public:
	void Hitted(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void MoveForward(float AxisValue);
    void MoveRight(float AxisValue);

	virtual void Action();
    virtual void Charge();
    virtual void Uncharge();
	virtual void Run();
	virtual void Walk();
};
