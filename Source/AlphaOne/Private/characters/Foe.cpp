#include "characters/Foe.h"
#include "widgets/HealthBar.h"
#include "Components/WidgetComponent.h"


AFoe::AFoe()
{
    SetType(EUnitTypes::Mob);
    DefaultFaction = EUnitFactions::Intruders;

    HealthBarUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar UI"));
    HealthBarUI->SetupAttachment(GetMesh());
}

void AFoe::BeginPlay()
{
    Super::BeginPlay();
    auto HealthBar = Cast<UHealthBar>(HealthBarUI->GetUserWidgetObject());
    HealthBarUI->SetVisibility(false);
    if (HealthBar) {
        HealthBar->SetAttributeSet(AttributeSet);
        HealthBar->SetOwningComponent(HealthBarUI);
    }
}