// Fill out your copyright notice in the Description page of Project Settings.


#include "widgets/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (HealthBar->Percent == 1.f) {
        FullHealthDuration += InDeltaTime;
    } else {
        FullHealthDuration = 0.f;
    }

    if (OwningComponent.IsValid() && OwningComponent->IsVisible() && (FullHealthDuration > 2.f)) {
        OwningComponent->SetVisibility(false);
    }
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

void UHealthBar::SetOwningComponent(UWidgetComponent* Comp)
{
    OwningComponent = Comp;
}

void UHealthBar::UpdatePercentage(float NewVal, float OldVal)
{
    auto percent = NewVal / AttributeSet->GetMaxHealth();
    HealthBar->SetPercent(percent);
    HealthPercentage->SetText(FText::Format(FText::AsCultureInvariant("{:d}%"), int32(percent * 100.f)));

    if (OwningComponent.IsValid() && !OwningComponent->IsVisible() && percent < 0.9999f) {
        OwningComponent->SetVisibility(true);
    }
}
