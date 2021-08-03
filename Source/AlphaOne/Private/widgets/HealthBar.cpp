// Fill out your copyright notice in the Description page of Project Settings.


#include "widgets/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

	if (!AttributeSet.IsValid()) return;

    auto percent = AttributeSet->GetHealth() / AttributeSet->GetMaxHealth();
    HealthBar->SetPercent(percent);

    FString PrintText = FString::Printf( TEXT("%0.1f %%"), percent * 100.f);
    HealthPercentage->SetText(FText::FromString(PrintText));
}

