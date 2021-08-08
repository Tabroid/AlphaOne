// A widget for the gauge of health/mana

#include "UI/widgets/PlayerStatusGauge.h"
#include "characters/CharacterBase.h"
#include "Components/ProgressBar.h"


void UPlayerStatusGauge::BindPlayerCharacter(ACharacterBase* Character)
{
    if (!IsValid(Character) || (Character == PlayerCharacter)) {
        return;
    }

    // remove the delegate
    if (IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetAttributes())) {
        auto OldAttr = PlayerCharacter->GetAttributes();
        OldAttr->RemoveHealthChangedDelegate("StatusGaugeUpdate");
        OldAttr->RemoveManaChangedDelegate("StatusGaugeUpdate");
        OldAttr->RemoveAbsorptionChangedDelegate("StatusGaugeUpdate");
    }

    PlayerCharacter = Character;
    auto Attr = PlayerCharacter->GetAttributes();

    FOnAttributeChanged HealthDelegate;
    HealthDelegate.BindUObject(this, &UPlayerStatusGauge::OnHealthChanged);
    Attr->AddHealthChangedDelegate(HealthDelegate, "StatusGaugeUpdate");
    OnHealthChanged(Attr->GetHealth(), Attr->GetHealth());

    FOnAttributeChanged ManaDelegate;
    HealthDelegate.BindUObject(this, &UPlayerStatusGauge::OnManaChanged);
    Attr->AddManaChangedDelegate(ManaDelegate, "StatusGaugeUpdate");
    OnHealthChanged(Attr->GetMana(), Attr->GetMana());

    FOnAttributeChanged AbsorptionDelegate;
    HealthDelegate.BindUObject(this, &UPlayerStatusGauge::OnAbsorptionChanged);
    Attr->AddAbsorptionChangedDelegate(AbsorptionDelegate, "StatusGaugeUpdate");
    OnHealthChanged(Attr->GetAbsorption(), Attr->GetAbsorption());
}

void UPlayerStatusGauge::OnHealthChanged(float NewVal, float OldVal)
{
    if (!IsValid(PlayerCharacter)) {
        return;
    }
    // should never get a negative or zero max health
    auto Percent = NewVal / PlayerCharacter->GetAttributes()->GetMaxHealth();
    HealthGauge->SetPercent(Percent);
}

void UPlayerStatusGauge::OnManaChanged(float NewVal, float OldVal)
{
    if (!IsValid(PlayerCharacter)) {
        return;
    }
    float MaxMana = PlayerCharacter->GetAttributes()->GetMaxMana();
    auto Percent = (MaxMana > 0.f) ? NewVal / MaxMana: 0.f;
    ManaGauge->SetPercent(Percent);
}

void UPlayerStatusGauge::OnAbsorptionChanged(float NewVal, float OldVal)
{
    if (!IsValid(PlayerCharacter)) {
        return;
    }
    // should never get a negative or zero max health
    float Percent = std::min(1.f, NewVal / PlayerCharacter->GetAttributes()->GetMaxHealth());
    AbsorptionGauge->SetPercent(Percent);
}
