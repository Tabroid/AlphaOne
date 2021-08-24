// Some globals to be used project-wise

#pragma once

#include "CoreMinimal.h"
#include "UnitAttributes.h"
#include "AlphaOneFaction.h"
#include "AlphaOneBattle.h"
#include "AlphaOneGameModeBase.h"
#include "AlphaOneInstance.h"


const static int32 WIDGET_ZORDER_DAMAGE_TEXT = 10;

#define COLLISION_PROJECTILE	ECC_GameTraceChannel1

UENUM(BlueprintType)
enum class ECardinalDirection : uint8
{
	North = 0,
    East = 1,
    South = 2,
    West = 3,
};

static inline ECardinalDirection AngleToDirection(float Angle)
{
    if (Angle >= -45.f && Angle < 45.f) {
        return ECardinalDirection::North;
    } else if (Angle >= 45.f && Angle < 135.f) {
        return ECardinalDirection::East;
    } else if (Angle >= 135.f || Angle < -135.f) {
        return ECardinalDirection::South;
    } else {
        return ECardinalDirection::West;
    }
}
