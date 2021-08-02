#include "AlphaOneInstance.h"

UAlphaOneInstance::UAlphaOneInstance()
{
    BattleInstance = CreateDefaultSubobject<UAlphaOneBattle>(TEXT("Battle"));
    static ConstructorHelpers::FObjectFinder<UDataTable> UnitDataObject(TEXT("DataTable'/Game/Data/UnitDataTable.UnitDataTable'"));
    if (UnitDataObject.Succeeded()) {
        UnitDataTable = UnitDataObject.Object;
    }
}

void UAlphaOneInstance::Shutdown()
{
    Super::Shutdown();
}
