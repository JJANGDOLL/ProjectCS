// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/CharacterBase.h"
#include "Utilities/Global.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    
}

void ACharacterBase::Hitted(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
    PrintLine();

    TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::MoveForward(float AxisValue)
{
    FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
    FVector direction = FQuat(rotator).GetForwardVector();
    AddMovementInput(direction, AxisValue);
}

void ACharacterBase::MoveRight(float AxisValue)
{
    FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
    FVector direction = FQuat(rotator).GetRightVector();
    AddMovementInput(direction, AxisValue);
}

void ACharacterBase::Action()
{
	PrintLine();
}

void ACharacterBase::Charge()
{
    PrintLine();
}

void ACharacterBase::Uncharge()
{
    PrintLine();
}

void ACharacterBase::Run()
{
    PrintLine();
}

void ACharacterBase::Walk()
{

}
