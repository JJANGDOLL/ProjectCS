// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Unarmed.h"
#include "Utilities/Global.h"
#include "../Structs/WeaponData.h"

AUnarmed::AUnarmed()
{
    Helpers::GetAsset(&WeaponData, TEXT("WeaponData'/Game/Datas/Weapon/UnarmedData.UnarmedData'"));

    if (!WeaponData)
        return;
}
