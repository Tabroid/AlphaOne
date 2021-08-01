#include "AlphaOneFaction.h"


UFactionComponent::UFactionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UFactionComponent::IsEnemy(const UFactionComponent* TargetFactionComponent) const
{
    if (TargetFactionComponent) {
        switch (Faction) {
        case EUnitFactions::Intruders:
            return TargetFactionComponent->GetFaction() != EUnitFactions::Intruders;
        default:
            return TargetFactionComponent->GetFaction() == EUnitFactions::Intruders;
        }
    }
    return false;
}

// Called when the game starts
void UFactionComponent::BeginPlay()
{
	Super::BeginPlay();
}
