// A widget for the gauge of health/mana

#include "UI/widgets/PlayerStatusGauge.h"
#include "characters/CharacterBase.h"
#include "Components/ProgressBar.h"


void UPlayerStatusGauge::BindPlayerCharacter(ACharacterBase* Character)
{
    if (!IsValid(Character) || (Character == PlayerCharacter)) {
        return;
    }

    UnBindPlayerCharacter();
    PlayerCharacter = Character;
    auto Attr = PlayerCharacter->GetAttributes();

    FOnAttributeChanged HealthDelegate;
    HealthDelegate.BindUObject(this, &UPlayerStatusGauge::OnHealthChanged);
    Attr->AddHealthChangedDelegate(HealthDelegate, "StatusGaugeUpdate");

    FOnAttributeChanged ManaDelegate;
    ManaDelegate.BindUObject(this, &UPlayerStatusGauge::OnManaChanged);
    Attr->AddManaChangedDelegate(ManaDelegate, "StatusGaugeUpdate");

    FOnAttributeChanged AbsorptionDelegate;
    AbsorptionDelegate.BindUObject(this, &UPlayerStatusGauge::OnAbsorptionChanged);
    Attr->AddAbsorptionChangedDelegate(AbsorptionDelegate, "StatusGaugeUpdate");

    // SetVisibility(ESlateVisibility::Visible);
}

void UPlayerStatusGauge::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayerStatusGauge::UnBindPlayerCharacter()
{
    // remove the delegate
    if (IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetAttributes())) {
        auto OldAttr = PlayerCharacter->GetAttributes();
        OldAttr->RemoveHealthChangedDelegate("StatusGaugeUpdate");
        OldAttr->RemoveManaChangedDelegate("StatusGaugeUpdate");
        OldAttr->RemoveAbsorptionChangedDelegate("StatusGaugeUpdate");
    }
    // SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerStatusGauge::OnHealthChanged(float NewVal, float OldVal)
{
    if (!IsValid(PlayerCharacter)) {
        return;
    }
    // should never get a negative or zero max health
    auto Percent = NewVal / PlayerCharacter->GetAttributes()->GetMaxHealth();
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
    //     FString::Printf(TEXT("Health Percentage %.2f / %.2f"), NewVal, PlayerCharacter->GetAttributes()->GetMaxHealth()));
    HealthGauge->SetPercent(Percent);
}

void UPlayerStatusGauge::OnManaChanged(float NewVal, float OldVal)
{
    if (!IsValid(PlayerCharacter)) {
        return;
    }
    float MaxMana = PlayerCharacter->GetAttributes()->GetMaxMana();
    auto Percent = (MaxMana > 0.f) ? NewVal / MaxMana: 0.f;
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
    //     FString::Printf(TEXT("Mana Percentage %.2f/ %.2f"), NewVal, PlayerCharacter->GetAttributes()->GetMaxMana()));
    ManaGauge->SetPercent(Percent);
}

void UPlayerStatusGauge::OnAbsorptionChanged(float NewVal, float OldVal)
{
    if (!IsValid(PlayerCharacter)) {
        return;
    }
    // should never get a negative or zero max health
    float Percent = (NewVal < 1e-5f) ? 0.f: std::min(1.f, NewVal / PlayerCharacter->GetAttributes()->GetMaxHealth());
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
    //     FString::Printf(TEXT("Absorption Percentage %.2f"), NewVal, PlayerCharacter->GetAttributes()->GetMaxHealth()));
    AbsorptionGauge->SetPercent(Percent);
}
