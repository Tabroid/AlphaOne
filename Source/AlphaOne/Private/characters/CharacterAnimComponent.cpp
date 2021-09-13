#include "characters/CharacterAnimComponent.h"
#include "characters/CharacterBase.h"
#include "Kismet/KismetMathLibrary.h"


static const float NearZero = 0.01f;

UCharacterAnimComponent::UCharacterAnimComponent()
    : bRotationCurveInit(0)
{
    // should be tick from owner
    PrimaryComponentTick.bCanEverTick = false;
    MyCharacter = Cast<ACharacterBase>(GetOwner());
}

void UCharacterAnimComponent::BeginPlay()
{
    Super::BeginPlay();
}

// update variables for animation graphs
void UCharacterAnimComponent::AnimationStatesUpdate(float DeltaTime)
{
	auto Velocity = MyCharacter->GetVelocity();
	auto VelocityRotX = UKismetMathLibrary::MakeRotFromX(Velocity);
	VelocitySize = Velocity.Size();

	// character is running
	auto Acceleration = MyCharacter->GetCharacterMovement()->GetCurrentAcceleration();
	AccelerationSize = Acceleration.Size();
	MyCharacter->SetAction(EUnitActions::Running, AccelerationSize > NearZero);

    auto AnimInstance = MyCharacter->GetMesh()->GetAnimInstance();

	if (AccelerationSize > NearZero && VelocitySize > NearZero) {
		AccDeltaRotator =  VelocityRotX - UKismetMathLibrary::MakeRotFromX(Acceleration);
		AccDeltaRotator.Normalize();
		if (!AnimInstance->GetCurveValue(MeleeTwistCurveName, MeleeTwist)) {
			MeleeTwist = 0.f;
		}
	} else {
		MeleeTwist = 0.f;
	}

	// aim delta, not dependent on states
	auto ActorRotation = MyCharacter->GetActorRotation();
	AimDeltaRotator = MyCharacter->GetBaseAimRotation() - ActorRotation;
	AimDeltaRotator.Normalize();

	// character has a velocity
	if (VelocitySize > NearZero) {
		// moving direction
		MoveDeltaRotator = VelocityRotX - ActorRotation;
		MoveDeltaRotator.Yaw -= MeleeTwist;
		MoveDeltaRotator.Normalize();
	}

	// yaw offset for mesh
	if (!MyCharacter->CheckAction(EUnitActions::Running) && !AnimInstance->IsAnyMontagePlaying()) {
        RotationYawOffset += RotationYawLastTick - ActorRotation.Yaw;
		RotationYawOffset = UKismetMathLibrary::NormalizeAxis(RotationYawOffset);
    } else {
		// RotationYawOffset = 0.f;
		; // RotationYawOffset = FMath::FInterpTo(RotationYawOffset, 0.f, DeltaTime, 5.f);
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%.2f"), RotationYawOffset));
    }

	RotationYawOffset = UKismetMathLibrary::NormalizeAxis(RotationYawOffset);
    JogAngle = MoveDeltaRotator.Yaw - RotationYawOffset;
    JogAngle = UKismetMathLibrary::NormalizeAxis(JogAngle);
    JogDirection = AngleToDirection(JogAngle, JogDirection, JogDirectionChangeTolerance);
	TurnInPlaceUpdate(DeltaTime);
	RotationYawLastTick = ActorRotation.Yaw;
}

// update turn in place offset
void UCharacterAnimComponent::TurnInPlaceUpdate(float DeltaTime)
{
    // check timer
    if (std::abs(RotationYawOffset - RotationYawOffsetLastTick) < NearZero) {
		RotationYawOffsetTimer += DeltaTime;
	} else {
		RotationYawOffsetTimer = 0.f;
    }

    // update states according to turn state
	if (MyCharacter->CheckAction(EUnitActions::Turning)) {
        auto AnimInstance = MyCharacter->GetMesh()->GetAnimInstance();
		float RotationCurveValue;
		if (AnimInstance->GetCurveValue(RotationCurveName, RotationCurveValue)) {
			float DeltaRotation = RotationCurveValue - RotationCurveValueLastTick;
			RotationCurveValueLastTick = RotationCurveValue;
			// rotation Rotation curve is always increasing
            if (DeltaRotation > 0.f && RotationCurveValue < 0.f) {
                if (!bRotationCurveInit) {
                    RotationDeltaMultiplier = RotationYawOffset / RotationCurveValueLastTick;
                    bRotationCurveInit = 1;
                    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
                    //     FString::Printf(TEXT("%.2f, %.2f, %.2f!"), RotationDeltaMultiplier, RotationYawOffset, RotationCurveValueLastTick));
                }
                RotationCurveValueSum += RotationDeltaMultiplier*DeltaRotation;
                RotationYawOffset += RotationDeltaMultiplier*DeltaRotation;
                // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
                //  	FString::Printf(TEXT("%.2f, %.2f, %.2f!"), RotationCurveValue, RotationDeltaMultiplier*DeltaRotation, RotationCurveValueSum));
            }
		}
		RotationCurveValueLastTick = RotationCurveValue;
	} else {
        RotationCurveValueSum = 0.f;
        TurnInPlaceType = AngleToTurnType(RotationYawOffset, TurnMinAngle, PivotMinAngle);
        if ((RotationYawOffsetTimer > TurnInPlaceTime) && (TurnInPlaceType != ETurnInPlaceTypes::Idle)) {
            MyCharacter->SetControl(EControlStates::WantsToTurn);
        }
        JogSpinType = CalculateSpinType(JogDirection, RotationYawOffset, JogSpinMinAngle);
        if (JogSpinType != EJogSpinTypes::None) {
            MyCharacter->SetControl(EControlStates::WantsToSpin);
        }
	}
    RotationYawOffsetLastTick = RotationYawOffset;
}


// *** blueprint pure functions below **

float UCharacterAnimComponent::CalculateLeanAngle(float LeanAngle, float DeltaTime, float LeanIntensity, float ChangeSpeed) const
{
    float LeanAngleTarget = - AccDeltaRotator.Yaw / DeltaTime * LeanIntensity;
    float VelocityScale = FMath::Clamp(VelocitySize / MyCharacter->GetMoveSpeed(), 0.f, 1.f);
    return FMath::FInterpTo(LeanAngle, LeanAngleTarget, DeltaTime, ChangeSpeed);
}

ECardinalDirections UCharacterAnimComponent::AngleToDirection(float NormalizedAngle, ECardinalDirections CurrentDirection, float ChangeTolerance)
{
    // maintain the current direction for a wider range
    switch (CurrentDirection) {
    case ECardinalDirections::North:
        if (NormalizedAngle < 45.f + ChangeTolerance && NormalizedAngle >= -45.f - ChangeTolerance) {
            return CurrentDirection;
        }
        break;
    case ECardinalDirections::East:
        if (NormalizedAngle < 135.f + ChangeTolerance && NormalizedAngle >= 45.f - ChangeTolerance) {
            return CurrentDirection;
        }
        break;
    case ECardinalDirections::South:
        if (NormalizedAngle < -135.f + ChangeTolerance || NormalizedAngle >= 135.f - ChangeTolerance) {
            return CurrentDirection;
        }
        break;
    case ECardinalDirections::West:
        if (NormalizedAngle < -45.f + ChangeTolerance && NormalizedAngle >= -135.f - ChangeTolerance) {
            return CurrentDirection;
        }
        break;
    }
    NormalizedAngle += 45.f;
    if (NormalizedAngle < 0.f) { NormalizedAngle += 360.f; }

    uint8 DirectionIndex = uint8(NormalizedAngle / 90.f);
    return static_cast<ECardinalDirections>(DirectionIndex);
}


ETurnInPlaceTypes UCharacterAnimComponent::AngleToTurnType(float NormalizedAngle, float TurnStart, float PivotStart)
{
    auto AbsAngle = std::abs(NormalizedAngle);
    if (AbsAngle >= PivotStart) {
        return (NormalizedAngle < 0.f) ? ETurnInPlaceTypes::RightPivot : ETurnInPlaceTypes::LeftPivot;
    } else if (AbsAngle >= TurnStart) {
        return (NormalizedAngle < 0.f) ? ETurnInPlaceTypes::RightTurn : ETurnInPlaceTypes::LeftTurn;
    }
    return ETurnInPlaceTypes::Idle;
}

EJogSpinTypes UCharacterAnimComponent::CalculateSpinType(ECardinalDirections CurrentDirection, float YawOffset, float SpinThreshold)
{
    if (std::abs(YawOffset) < SpinThreshold) {
        return EJogSpinTypes::None;
    }

    switch (CurrentDirection) {
    case ECardinalDirections::North:
        return (YawOffset < 0.f) ? EJogSpinTypes::FwdToBwd_CW : EJogSpinTypes::FwdToBwd_CCW;
    case ECardinalDirections::South:
        return (YawOffset < 0.f) ? EJogSpinTypes::BwdToFwd_CW : EJogSpinTypes::BwdToFwd_CCW;
    default:
        break;
    }
    return EJogSpinTypes::None;
}
