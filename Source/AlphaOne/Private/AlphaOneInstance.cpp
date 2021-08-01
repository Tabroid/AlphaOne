#include "AlphaOneInstance.h"

UAlphaOneInstance::UAlphaOneInstance()
{
    BattleInstance = CreateDefaultSubobject<UAlphaOneBattle>(TEXT("Battle"));
}

void UAlphaOneInstance::Shutdown()
{
    Super::Shutdown();
}
