// Fill out your copyright notice in the Description page of Project Settings.


#include "widgets/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "buildings/Tower.h"

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

	if (!AttributeSet.IsValid()) return;

    auto percentage = AttributeSet->GetHealth() / AttributeSet->GetMaxHealth() * 100.f;
    HealthBar->SetPercent(percentage / 100.0f);

    FString PrintText = FString::Printf( TEXT("%0.1f %%"), percentage);
    HealthPercentage->SetText(FText::FromString(PrintText));
}

