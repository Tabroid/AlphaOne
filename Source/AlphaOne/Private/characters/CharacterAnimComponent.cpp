#include "characters/CharacterAnimComponent.h"
#include "characters/CharacterBase.h"
#include "Kismet/KismetMathLibrary.h"


static const float NearZero = 0.01f;

UCharacterAnimComponent::UCharacterAnimComponent()
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
		MoveDirection = UAlphaOneMath::AngleToDirection(MoveDeltaRotator.Yaw, MoveDirection);
	}

	// yaw offset for mesh
	if (MyCharacter->CheckAction(EUnitActions::Running) ||  AnimInstance->IsAnyMontagePlaying()) {
		// RotationYawOffset = 0.f;
		RotationYawOffset = FMath::FInterpTo(RotationYawOffset, 0.f, DeltaTime, 5.f);
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%.2f"), RotationYawOffset));
	} else {
		RotationYawOffset += RotationYawLastTick - ActorRotation.Yaw;
		RotationYawOffset = UKismetMathLibrary::NormalizeAxis(RotationYawOffset);
	}

	// RotationYawOffset += MeleeTwist;
	// update last tick information in the end
	TurnInPlaceUpdate(DeltaTime);
	if (std::abs(RotationYawOffset - RotationYawOffsetLastTick) < NearZero) {
		RotationYawOffsetTimer += DeltaTime;
	} else {
		RotationYawOffsetTimer = 0.f;
	}
	RotationYawOffset = UKismetMathLibrary::NormalizeAxis(RotationYawOffset);
	RotationYawLastTick = ActorRotation.Yaw;
	RotationYawOffsetLastTick = RotationYawOffset;
}

// update turn in place offset
void UCharacterAnimComponent::TurnInPlaceUpdate(float DeltaTime)
{
	if (MyCharacter->CheckAction(EUnitActions::Turning)) {
        auto AnimInstance = MyCharacter->GetMesh()->GetAnimInstance();
		float DistanceCurveValue;
		if (AnimInstance->GetCurveValue(DistanceCurveName, DistanceCurveValue)) {
			float DeltaDistance = DistanceCurveValue - DistanceCurveValueLastTick;
			DistanceCurveValueLastTick = DistanceCurveValue;
			// rotation distance curve is always increasing
			if (DeltaDistance > 0.f) {
				DistanceCurveValueSum += DistanceDeltaMultiplier*DeltaDistance;
				RotationYawOffset += DistanceDeltaMultiplier*DeltaDistance;
				// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
				//  	FString::Printf(TEXT("%.2f, %.2f, %.2f!"), DistanceCurveValue, DistanceCurveValueLastTick - DistanceCurveValue, DistanceCurveValueSum));
			}
		}
		DistanceCurveValueLastTick = DistanceCurveValue;
	} else {
		DistanceCurveValueSum = 0.f;
	}
}
