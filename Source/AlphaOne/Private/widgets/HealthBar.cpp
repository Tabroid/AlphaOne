// Fill out your copyright notice in the Description page of Project Settings.


#include "widgets/HealthBar.h"
#include "characters/CharacterBase.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (HealthBar->Percent == 1.f) {
        FullHealthDuration += InDeltaTime;
    } else {
        FullHealthDuration = 0.f;
    }

    if (OwningComponent.IsValid() && OwningComponent->IsVisible()) {
        // hide
        if (FullHealthDuration > ALPHAONE_FULL_HEALTH_HIDDEN_TICK) {
            OwningComponent->SetVisibility(false);
        // rotation
        } else {
            auto Actor = OwningComponent->GetOwner();
            auto PlayerCharacter = UGameplayStatics::GetPlayerCharacter(Actor->GetWorld(), 0);
            if (PlayerCharacter) {
                FVector LookDirection = PlayerCharacter->GetActorLocation() - Actor->GetActorLocation();
	            OwningComponent->SetWorldRotation(LookDirection.Rotation());
            }
        }
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
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Update Percentage: %d, %d!"), int(NewVal), int(OldVal)));
    float MaxHealth = AttributeSet->GetMaxHealth();
    auto Percent = (MaxHealth > 0.f) ? NewVal / MaxHealth: 0.f;
    HealthBar->SetPercent(Percent);
    HealthPercentage->SetText(FText::Format(FText::AsCultureInvariant("{:d}%"), int32(Percent * 100.f)));

    if (OwningComponent.IsValid() && !OwningComponent->IsVisible() && Percent < 0.9999f) {
        OwningComponent->SetVisibility(true);
    }
}
