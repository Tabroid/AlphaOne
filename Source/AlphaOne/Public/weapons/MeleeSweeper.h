#pragma once

#include "AlphaOne.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "weapons/WeaponBase.h"
#include "MeleeSweeper.generated.h"


UCLASS()
class ALPHAONE_API UMeleeSweeper : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyTick(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation) override;
	virtual void NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation, float TotalDuration) override;

	// a sweep check interval, default for 60 fps
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float SweepingInterval = 0.015f;
	float SweepingTimer = 0.f;

	TArray<FHitResult> HitResult;
	TArray<AActor*> HitActors;
	TArray<AActor*> IgnoreActors;
	TArray<FVector> SocketLocations_Prev;

	AWeaponBase* Weapon;
	TSubclassOf<UDamageType> DamageType;
};
