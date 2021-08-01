#include "AlphaOneFaction.h"


UFactionComponent::UFactionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UFactionComponent::IsEnemy(const AActor *Target) const
{
    auto FactorComponent = Target->FindComponentByClass<UFactionComponent>();
    if (FactorComponent) {
        switch (Faction) {
        case EUnitFactions::Intruders:
            return FactorComponent->GetFaction() != EUnitFactions::Intruders;
        default:
            return FactorComponent->GetFaction() == EUnitFactions::Intruders;
        }
    }
    return false;
}

// Called when the game starts
void UFactionComponent::BeginPlay()
{
	Super::BeginPlay();
}
