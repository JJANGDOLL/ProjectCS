// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/CharacterBase.h"
#include "Components/TimelineComponent.h"
#include "../System/IDamageable.h"
#include "CharacterCore.generated.h"

#define ADD_AXIS_KEY(VarName, KeyName, Key, Value, Method, InputComponent) \
FInputAxisKeyMapping VarName(KeyName,  Key, Value); \
InputComponent->BindAxis(KeyName, this, Method); \
GetWorld()->GetFirstPlayerController()->PlayerInput->AddAxisMapping(VarName);

#define ADD_ACTION_KEY(VarName, KeyName, Key, bIsShift, bIsControl, bIsAlt, bIsCommand, KeyEvent, Method, InputComponent) \
FInputActionKeyMapping VarName(KeyName, Key, bIsShift, bIsControl, bIsAlt, bIsCommand); \
PlayerInputComponent->BindAction(KeyName, KeyEvent, this, Method); \
GetWorld()->GetFirstPlayerController()->PlayerInput->AddActionMapping(VarName);

DECLARE_DELEGATE(FWeaponChangeDelegate);

class USpringArmComponent;
class UCameraComponent;
class AWeaponCore;

UCLASS()
class PROJECTCS_API ACharacterCore : public ACharacterBase
{
    GENERATED_BODY()

public:
    ACharacterCore();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


    float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


protected:
    void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable)
    void SetText(FText InName);

private:
    UPROPERTY(VisibleDefaultsOnly)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleDefaultsOnly)
    UCameraComponent* Camera;

    UPROPERTY(VisibleDefaultsOnly)
    class UTextRenderComponent* Name;


private:
    AWeaponCore* UnarmedWeapon;
    AWeaponCore* MeleeWeapon;
    AWeaponCore* RangeWeapon;
    AWeaponCore* CurrentWeapon;

public:
    FWeaponChangeDelegate OnWeaponChanged;

private:
    float FieldOfView = 90.f;
    float FieldOfViewBerf = 0.f;
    float WalkSpeed = 300.f;
    float SpeedBerf = 0.f;
    float Appearance = 0.f;

public:
    void Test();
    virtual void Action() override;
    virtual void Charge() override;
    virtual void Uncharge() override;
    virtual void Run() override;
    virtual void Walk() override;

private:
    inline bool CanWeaponChange() { return bCanWeaponChange; }
    void EquipUnarmed();
    void EquipMelee();
    void EquipRange();
    void Transform();

private:
    class UCurveFloat* AppearanceCurve;
    FTimeline AppearanceTL;
    FOnTimelineFloat AppearanceTLFloat;
    FOnTimelineEvent AppearanceTLFinish;

private:
    UFUNCTION()
    void SetAppearance(float value);

    UFUNCTION()
    void EndAppearance();

public:
    AWeaponCore* GetCurrentWeapon();

    void BindWidget(TSubclassOf<class UUserWidget> InWidget);

private:
    class UCharacterData* DefaultCharacterData;
    class UMaterialInstanceDynamic* BodyMaterial;
    bool bCanWeaponChange = true;
    class UMainInterface* MainUI;

    TSubclassOf<UMainInterface> PlayerHUDClass;
    UMainInterface* PlayerHUD;

    float MaxHealth = 200.f;
    float CurHealth = 200.f;

    UPROPERTY()
    UUserWidget* WidgetInstance;
};
