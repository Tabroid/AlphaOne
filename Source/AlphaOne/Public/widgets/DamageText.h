// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "Blueprint/UserWidget.h"
#include "DamageText.generated.h"

class UTextBlock;
class APlayerController;

UCLASS()
class ALPHAONE_API UDamageText : public UUserWidget
{
    GENERATED_BODY()

public:
    void PopText(const APlayerController* Player, const FText& Text, FVector Position,
                 FLinearColor Color = FLinearColor::White, bool Parabola = true, float Life = 1.f, float Distance = 200.f);

protected:
    void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

    float Timer = 0.f;
    float LifeSpan = 2.0f;
    FVector2D Velocity = {0.f, 0.f};
    FVector2D Acceleration = {0.f, 0.f};
    FVector2D ViewPosition;

    UPROPERTY (meta = (BindWidget))
    UTextBlock* DamageText = nullptr;
};
