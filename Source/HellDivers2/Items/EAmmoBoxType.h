// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAmmoBoxType : uint8
{
	Magazine = 0,
	Syringe,
	Grenade,
	StratagemGrenade
};