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

// Convert absolute angle to cardinal direction and relative angle
// Stay the same direction for a wider range to avoid a clear separation point
// Input angle is normalized (-180 to 180)
static ECardinalDirection AngleToDirection(float NormalizedAngle, ECardinalDirection CurrentDirection)
{
    // maintain the current direction for a wider range
    switch (CurrentDirection) {
    case ECardinalDirection::North:
        if (NormalizedAngle < 60.f && NormalizedAngle >= -60.f) {
            return CurrentDirection;
        }
        break;
    case ECardinalDirection::East:
        if (NormalizedAngle < 130.f && NormalizedAngle >= 30.f) {
            return CurrentDirection;
        }
        break;
    case ECardinalDirection::South:
        if (NormalizedAngle < -120.f || NormalizedAngle >= 120.f) {
            return CurrentDirection;
        }
        break;
    case ECardinalDirection::West:
        if (NormalizedAngle < -30.f && NormalizedAngle >= -130.f) {
            return CurrentDirection;
        }
        break;
    }
    NormalizedAngle += 45.f;
    if (NormalizedAngle < 0.f) { NormalizedAngle += 360.f; }

    uint8 DirectionIndex = uint8(NormalizedAngle / 90.f);
    return static_cast<ECardinalDirection>(DirectionIndex);
}
