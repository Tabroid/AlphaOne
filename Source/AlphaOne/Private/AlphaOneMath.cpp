#include "AlphaOneMath.h"


ECardinalDirections UAlphaOneMath::AngleToDirection(float NormalizedAngle, ECardinalDirections CurrentDirection, float ChangeTolerance)
{
    // maintain the current direction for a wider range
    switch (CurrentDirection) {
    case ECardinalDirections::North:
        if (NormalizedAngle < 45.f + ChangeTolerance && NormalizedAngle >= -45.f - ChangeTolerance) {
            return CurrentDirection;
        }
        break;
    case ECardinalDirections::East:
        if (NormalizedAngle < 135.f - ChangeTolerance && NormalizedAngle >= 45.f - ChangeTolerance) {
            return CurrentDirection;
        }
        break;
    case ECardinalDirections::South:
        if (NormalizedAngle < -135.f + ChangeTolerance || NormalizedAngle >= 135.f - ChangeTolerance) {
            return CurrentDirection;
        }
        break;
    case ECardinalDirections::West:
        if (NormalizedAngle < -45.f + ChangeTolerance && NormalizedAngle >= -135.f + ChangeTolerance) {
            return CurrentDirection;
        }
        break;
    }
    NormalizedAngle += 45.f;
    if (NormalizedAngle < 0.f) { NormalizedAngle += 360.f; }

    uint8 DirectionIndex = uint8(NormalizedAngle / 90.f);
    return static_cast<ECardinalDirections>(DirectionIndex);
}


ETurnInPlaceTypes UAlphaOneMath::AngleToTurnType(float NormalizedAngle, float TurnStart, float PivotStart)
{
    auto AbsAngle = std::abs(NormalizedAngle);
    if (AbsAngle >= PivotStart) {
        return (NormalizedAngle < 0.f) ? ETurnInPlaceTypes::RightPivot : ETurnInPlaceTypes::LeftPivot;
    } else if (AbsAngle >= TurnStart) {
        return (NormalizedAngle < 0.f) ? ETurnInPlaceTypes::RightTurn : ETurnInPlaceTypes::LeftTurn;
    }
    return ETurnInPlaceTypes::Idle;
}