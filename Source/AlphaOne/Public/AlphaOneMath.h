// a collection of functions to help calculations
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceProxy.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AlphaOneMath.generated.h"


UENUM(BlueprintType)
enum class ECardinalDirections : uint8
{
	North = 0,
    East = 1,
    South = 2,
    West = 3,
};

UENUM(BlueprintType)
enum class ETurnInPlaceTypes : uint8
{
	Idle = 0,
    RightTurn = 1,
    RightPivot = 2,
    LeftTurn = 3,
    LeftPivot = 4,
};

UCLASS()
class ALPHAONE_API UAlphaOneMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
    // Convert absolute angle to cardinal direction. Stay the same direction for a wider range defined by the tolerance.
    // Assuming input angle is normalized to axis (-180 to 180)
    UFUNCTION(BlueprintCallable, Category = "AlphaOne Math")
    static ECardinalDirections AngleToDirection(float NormalizedAngle, ECardinalDirections CurrentDirection, float ChangeTolerance = 15.f);

    // Determine the turn in place type according to the offset angle
    // Assuming input angle is normalized to axis (-180 to 180)
    UFUNCTION(BlueprintCallable, Category = "AlphaOne Math")
    static ETurnInPlaceTypes AngleToTurnType(float NormalizedAngle, float TurnStart = 75.f, float PivotStart = 150.f);
};

