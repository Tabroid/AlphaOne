// A character component to manage animations

#pragma once

#include "AlphaOne.h"
#include "Components/ActorComponent.h"
#include "CharacterAnimComponent.generated.h"

class ACharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALPHAONE_API UCharacterAnimComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCharacterAnimComponent();

    virtual void AnimationStatesUpdate(float DeltaTime);

    virtual void BeginPlay() override;
    virtual bool IsNavigationRelevant() const override { return true; }

protected:
    virtual void TurnInPlaceUpdate(float DeltaTime);

    ACharacterBase* MyCharacter;

	float RotationYawLastTick = 0.f;
	float RotationCurveValueLastTick = 0.f;
	float RotationCurveValueSum = 0.f;
	float RotationYawOffsetLastTick = 0.f;
    float RotationDeltaMultiplier = 1.f;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float RotationYawOffsetTimer = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float VelocitySize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float AccelerationSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	ECardinalDirections MoveDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FRotator AimDeltaRotator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FRotator MoveDeltaRotator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FRotator AccDeltaRotator;
};