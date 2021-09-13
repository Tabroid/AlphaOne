// A character component to manage animations

#pragma once

#include "AlphaOne.h"
#include "Components/ActorComponent.h"
#include "CharacterAnimComponent.generated.h"

class ACharacterBase;

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

UENUM(BlueprintType)
enum class EJogSpinTypes : uint8
{
    None = 0,
    FwdToBwd_CW = 1,
    FwdToBwd_CCW = 2,
    BwdToFwd_CW = 3,
    BwdToFwd_CCW = 4,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALPHAONE_API UCharacterAnimComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCharacterAnimComponent();

    virtual void AnimationStatesUpdate(float DeltaTime);

    virtual void BeginPlay() override;
    virtual bool IsNavigationRelevant() const override { return true; }

    UFUNCTION(BlueprintCallable)
    void TurnInPlaceStart();

    UFUNCTION(BlueprintCallable)
    void TurnInPlaceEnd();

    UFUNCTION(BlueprintCallable)
    void JogSpinStart();

    UFUNCTION(BlueprintCallable)
    void JogSpinEnd();

    // *** some parameters ***
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Parameter")
    float TurnInPlaceTime = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Parameter")
    float JogDirectionChangeTolerance = 15.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Parameter")
    float TurnMinAngle = 75.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Parameter")
    float PivotMinAngle = 75.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Parameter")
    float JogSpinMinAngle = 90.f;

protected:
    virtual void TurnInPlaceUpdate(float DeltaTime);

    ACharacterBase* MyCharacter;

    float RotationYawLastTick = 0.f;
    float RotationYawOffsetLastTick = 0.f;
    float RotationYawOffsetTimer = 0.f;

       float RotationCurveValueLastTick = 0.f;
    float RotationCurveValueSum = 0.f;
    float RotationDeltaMultiplier = 1.;

    float JogSpinAngleStart = 0.f;
    float JogSpinRotationStart = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    FName RotationCurveName = "DistanceCurve";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    uint8 bRotationCurveInit : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    FName MeleeTwistCurveName = "MeleeTwist";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    float MeleeTwist = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    float RotationYawOffset = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    ETurnInPlaceTypes TurnInPlaceType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    float VelocitySize;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    float AccelerationSize;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    FRotator AimDeltaRotator;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    FRotator MoveDeltaRotator;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    FRotator AccDeltaRotator;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    ECardinalDirections JogDirection;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    float JogAngle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    EJogSpinTypes JogSpinType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    float JogSpinAngle;


// blueprint pure functions to help calculate animaton states ***
public:
    // Determine the lean angle
    UFUNCTION(BlueprintPure, Category = "AlphaOne Math")
    float CalculateLeanAngle(float LeanAngle, float DeltaTime, float LeanIntensity = 0.08f, float ChangeSpeed = 6.f) const;

    // Convert absolute angle to cardinal direction. Stay the same direction for a wider range defined by the tolerance.
    // Assuming input angle is normalized to axis (-180 to 180)
    UFUNCTION(BlueprintPure, Category = "AlphaOne Math")
    static ECardinalDirections AngleToDirection(float NormalizedAngle, ECardinalDirections CurrentDirection, float ChangeTolerance = 15.f);

    // Determine the turn in place type according to the offset angle
    // Assuming input angle is normalized to axis (-180 to 180)
    UFUNCTION(BlueprintPure, Category = "AlphaOne Math")
    static ETurnInPlaceTypes AngleToTurnType(float NormalizedAngle, float TurnStart = 75.f, float PivotStart = 150.f);

    // Determine the jog spin state
    UFUNCTION(BlueprintPure, Category = "AlphaOne Math")
    static EJogSpinTypes CalculateSpinType(ECardinalDirections CurrentDirection, float YawOffset, float SpinThreshold = 90.f);

    UFUNCTION(BlueprintPure, Category = "AlphaOne Math")
    static float CalculateJogSpinAngle(EJogSpinTypes CurrentSpinType, float CurrentAngle);

};
