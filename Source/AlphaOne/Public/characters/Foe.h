// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "characters/CharacterBase.h"
#include "Foe.generated.h"

class UWidgetComponent;

UCLASS()
class ALPHAONE_API AFoe : public ACharacterBase
{
    GENERATED_BODY()

public:
    AFoe();

    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UWidgetComponent* HealthBarUI = nullptr;
};
