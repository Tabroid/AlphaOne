// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "buildings/BuildingBase.h"
#include "Tower.generated.h"

class UCapsuleComponent;
class UWidgetComponent;


UCLASS()
class ALPHAONE_API ATower : public ABuildingBase
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATower();
};
