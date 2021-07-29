// a header for globals for the whole project
# pragma once

#include "CoreMinimal.h"
#include "AlphaOneGlobals.generated.h"

// place holder
UENUM(BlueprintType)
enum class EUnitTypes: uint8
{
    UNDEFINED = 0   UMETA(Hidden),
    Hero	  = 1   UMETA(DisplayName = "Hero"),
    Building  = 2   UMETA(DisplayName = "Building"),
    Mob       = 3   UMETA(DisplayName = "Mob"),
    Elite	  = 4   UMETA(DisplayName = "Elite"),
    Boss      = 5   UMETA(DisplayName = "Boss"),
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EUnitActions: uint8
{
    NONE            = 0          UMETA(Hidden),
    Attacking       = 1 << 0     UMETA(DisplayName = "Attacking"),
    Running         = 1 << 1     UMETA(DisplayName = "Running"),
    BeingAttacked   = 1 << 2     UMETA(DisplayName = "BeingAttacked"),
    Dying           = 1 << 3     UMETA(DisplayName = "Dying"),
};
ENUM_CLASS_FLAGS(EUnitActions);


UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EUnitStatuses: uint8
{
    NONE     = 0          UMETA(Hidden),
    Dead     = 1 << 0     UMETA(DisplayName = "Dead"),
};
ENUM_CLASS_FLAGS(EUnitStatuses);

USTRUCT(BlueprintType)
struct FWeaponSockets
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnParams)
    FName Name = "";
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnParams)
	float Radius = 0;
};
