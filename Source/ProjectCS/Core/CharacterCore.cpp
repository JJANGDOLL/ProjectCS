// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCore.h"
#include "GameFramework/PlayerInput.h"
#include "Structs/CharacterData.h"
#include "Utilities/Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Weapons/Unarmed.h"
#include "../Weapons/Bow.h"
#include "../Weapons/Katana.h"
#include "WeaponCore.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "../Widgets/MainInterface.h"
#include "GameFramework/PlayerController.h"
#include "../Widgets/PlayerStatus.h"
#include "Components/TextRenderComponent.h"
#include "Blueprint/UserWidget.h"

ACharacterCore::ACharacterCore()
{
    Helpers::CreateComponent(this, &SpringArm, "SpringArm", GetCapsuleComponent());
    Helpers::CreateComponent(this, &Camera, "Camera", SpringArm);

    Helpers::GetAsset(&DefaultCharacterData, TEXT("CharacterData'/Game/Datas/Character/DefaultCharacter.DefaultCharacter'"));

    if (!DefaultCharacterData)
        return;

    GetMesh()->SetSkeletalMesh(DefaultCharacterData->CharacterBody);
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
    GetMesh()->SetAnimInstanceClass(DefaultCharacterData->CharacterAnimInst);

    bUseControllerRotationYaw = true;
    SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 45.f));
    SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    SpringArm->TargetArmLength = 200.f;
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->bDoCollisionTest = true;
    SpringArm->bEnableCameraLag = false;
    SpringArm->SocketOffset = FVector(0.f, 75.f, 35.f);

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->MaxWalkSpeed = 300.f;
    GetCharacterMovement()->MaxAcceleration = 1024.f;

    Camera->FieldOfView = FieldOfView;

    Helpers::GetAsset(&AppearanceCurve, TEXT("CurveFloat'/Game/Assets/Character/AppearanceCurve.AppearanceCurve'"));

    PlayerHUD = nullptr;
    Helpers::GetClass<UMainInterface>(&PlayerHUDClass, TEXT("WidgetBlueprint'/Game/Assets/Character/WBP_MainInterface.WBP_MainInterface_C'"));

    GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerBody"));

    Helpers::CreateComponent(this, &Name, TEXT("Name"), GetCapsuleComponent());

    Name->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    Name->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
    Name->SetWorldSize(10.f);
    Name->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
    Name->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 15.f));
}

void ACharacterCore::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

//     Helpers::DrawGizmo(GetWorld(), GetActorLocation() + GetActorForwardVector() * 250.f, FQuat(GetController()->GetControlRotation()));
//     Logger::Log(GetController()->GetControlRotation());
//     Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, SkeletalMeshArms->GetAnimInstance()->GetCurveValue(TEXT("Field Of View")), GetWorld()->GetDeltaSeconds(), 10.f));

    Camera->SetFieldOfView(UKismetMathLibrary::FInterpTo(Camera->FieldOfView, FieldOfView - GetMesh()->GetAnimInstance()->GetCurveValue(TEXT("FieldOfView")), DeltaTime, 5.f));
    GetCharacterMovement()->MaxWalkSpeed = UKismetMathLibrary::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, WalkSpeed + SpeedBerf, DeltaTime, 5.f);

    AppearanceTL.TickTimeline(DeltaTime);
}

void ACharacterCore::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    ADD_AXIS_KEY(forwardKey, "Forward", EKeys::W, 1.f, &ACharacterBase::MoveForward, PlayerInputComponent);
    ADD_AXIS_KEY(backKey, "Back", EKeys::S, -1.f, &ACharacterBase::MoveForward, PlayerInputComponent);
    ADD_AXIS_KEY(rightKey, "Right", EKeys::D, 1.f, &ACharacterBase::MoveRight, PlayerInputComponent);
    ADD_AXIS_KEY(leftKey, "Left", EKeys::A, -1.f, &ACharacterBase::MoveRight, PlayerInputComponent);
    ADD_AXIS_KEY(lookUpKey, "LookUp", EKeys::MouseY, -1.f, &APawn::AddControllerPitchInput, PlayerInputComponent);
    ADD_AXIS_KEY(turnKey, "Turn", EKeys::MouseX, 1.f, &APawn::AddControllerYawInput, PlayerInputComponent);

    ADD_ACTION_KEY(testKey, "Test", EKeys::P, false, false, false, false, IE_Pressed, &ACharacterCore::Test, PlayerInputComponent);
    ADD_ACTION_KEY(actionKey, "Action", EKeys::LeftMouseButton, false, false, false, false, IE_Pressed, &ACharacterBase::Action, PlayerInputComponent);
    ADD_ACTION_KEY(chargeKey, "Charge", EKeys::RightMouseButton, false, false, false, false, IE_Pressed, &ACharacterBase::Charge, PlayerInputComponent);
    ADD_ACTION_KEY(unchargeKey, "Uncharge", EKeys::RightMouseButton, false, false, false, false, IE_Released, &ACharacterBase::Uncharge, PlayerInputComponent);
    ADD_ACTION_KEY(runKey, "Run", EKeys::LeftShift, false, false, false, false, IE_Pressed, &ACharacterBase::Run, PlayerInputComponent);
    ADD_ACTION_KEY(walkKey, "Walk", EKeys::LeftShift, false, false, false, false, IE_Released, &ACharacterBase::Walk, PlayerInputComponent);

    ADD_ACTION_KEY(meleeKey, "Melee", EKeys::One, false, false, false, false, IE_Pressed, &ACharacterCore::EquipMelee, PlayerInputComponent);
    ADD_ACTION_KEY(rangeKey, "Range", EKeys::Two, false, false, false, false, IE_Pressed, &ACharacterCore::EquipRange, PlayerInputComponent);
    ADD_ACTION_KEY(unarmKey, "Unarm", EKeys::Three, false, false, false, false, IE_Pressed, &ACharacterCore::EquipUnarmed, PlayerInputComponent);
}

float ACharacterCore::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    PrintLine();

    Logger::Log(CurHealth);

    float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

    CurHealth = FMath::Clamp<float>(CurHealth - damage, 0, MaxHealth);
    PlayerHUD->GetPlayerStatus()->SetHealth(MaxHealth, CurHealth);

    return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void ACharacterCore::Test()
{
    PrintLine();
}

void ACharacterCore::Action()
{
    Super::Action();

    CurrentWeapon->Action();
}

void ACharacterCore::Charge()
{
    Super::Charge();

    CurrentWeapon->Charge();
}

void ACharacterCore::Uncharge()
{
    Super::Uncharge();

    CurrentWeapon->Uncharge();
}

void ACharacterCore::Run()
{
    SpeedBerf = 100.f;
}

void ACharacterCore::Walk()
{
    SpeedBerf = 0.f;
}

void ACharacterCore::EquipUnarmed()
{
    if (!CanWeaponChange())
        return;

    if (CurrentWeapon)
        CurrentWeapon->BeUnequipped();
    Transform();
    CurrentWeapon = UnarmedWeapon;
    OnWeaponChanged.ExecuteIfBound();
    CurrentWeapon->BeEquipped();
}

void ACharacterCore::EquipMelee()
{
    if (!CanWeaponChange())
        return;

    if (CurrentWeapon)
        CurrentWeapon->BeUnequipped();
    Transform();
    CurrentWeapon = MeleeWeapon;
    OnWeaponChanged.ExecuteIfBound();
    CurrentWeapon->BeEquipped();

}

void ACharacterCore::EquipRange()
{
    if (!CanWeaponChange())
        return;

    if (CurrentWeapon)
        CurrentWeapon->BeUnequipped();
    Transform();
    CurrentWeapon = RangeWeapon;
    OnWeaponChanged.ExecuteIfBound();
    CurrentWeapon->BeEquipped();
}

void ACharacterCore::Transform()
{
    bCanWeaponChange = false;
    AppearanceTL.PlayFromStart();
}

void ACharacterCore::SetAppearance(float value)
{
    BodyMaterial->SetScalarParameterValue("Appearance", value);
}

void ACharacterCore::EndAppearance()
{
    bCanWeaponChange = true;
}

AWeaponCore* ACharacterCore::GetCurrentWeapon()
{
    return CurrentWeapon;
}

void ACharacterCore::BindWidget(TSubclassOf<UUserWidget> InWidget)
{
}

void ACharacterCore::BeginPlay()
{
    Super::BeginPlay();

    UnarmedWeapon = GetWorld()->SpawnActor<AWeaponCore>(AUnarmed::StaticClass(), FTransform());
    UnarmedWeapon->SetOwner(this);
    MeleeWeapon = GetWorld()->SpawnActor<AWeaponCore>(AKatana::StaticClass(), FTransform());
    MeleeWeapon->SetOwner(this);
    RangeWeapon = GetWorld()->SpawnActor<AWeaponCore>(ABow::StaticClass(), FTransform());
    RangeWeapon->SetOwner(this);

    EquipUnarmed();

    UMaterialInstanceConstant* body = nullptr;
    Helpers::GetAssetDynamic(&body, TEXT("MaterialInstanceConstant'/Game/GhostSamurai_Bundle/Mannequin/Character/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'"));

    BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
    GetMesh()->SetMaterial(0, BodyMaterial);

    AppearanceTLFloat.BindUFunction(this, "SetAppearance");
    AppearanceTLFinish.BindUFunction(this, "EndAppearance");
    AppearanceTL.AddInterpFloat(AppearanceCurve, AppearanceTLFloat);
    AppearanceTL.SetTimelineFinishedFunc(AppearanceTLFinish);

    APlayerController* FPC = GetController<APlayerController>();
    PlayerHUD = CreateWidget<UMainInterface>(FPC, PlayerHUDClass);
    verifyf(PlayerHUD, L"Invalid HUD");
    PlayerHUD->AddToPlayerScreen();

    PlayerHUD->GetPlayerStatus()->SetHealth(MaxHealth, CurHealth);
}

void ACharacterCore::SetText(FText InName)
{
    Name->SetText(InName);
}

