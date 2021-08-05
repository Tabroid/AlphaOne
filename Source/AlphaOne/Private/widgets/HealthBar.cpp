// Fill out your copyright notice in the Description page of Project Settings.


#include "widgets/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHealthBar::BeginDestroy()
{
    // remove the delegate
    if (AttributeSet.IsValid()) {
        AttributeSet->RemoveHealthChangedDelegate("HealthBarUpdate");
    }
    Super::BeginDestroy();
}

void UHealthBar::SetAttributeSet(UCharacterAttributes* Attr)
{
    if (!IsValid(Attr) || (Attr == AttributeSet)) {
        return;
    }

    // remove the delegate
    if (AttributeSet.IsValid()) {
        AttributeSet->RemoveHealthChangedDelegate("HealthBarUpdate");
    }

    AttributeSet = Attr;
    FOnAttributeChanged Delegate;
    Delegate.BindUObject(this, &UHealthBar::UpdatePercentage);
    AttributeSet->AddHealthChangedDelegate(Delegate, "HealthBarUpdate");
    UpdatePercentage(AttributeSet->GetHealth(), 0.f);
}

void UHealthBar::UpdatePercentage(float NewVal, float OldVal)
{
    auto percent = NewVal / AttributeSet->GetMaxHealth();
    HealthBar->SetPercent(percent);

    HealthPercentage->SetText(FText::Format(FText::AsCultureInvariant("{:d}%"), int32(percent * 100.f)));
}