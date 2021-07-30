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
	TArray<FHitResult> HitResult;
	TArray<AActor*> HitActors;
	TArray<AActor*> IgnoreActors;
	TArray<FVector> SocketLocations_Prev;

	AWeaponBase* Weapon;
	TSubclassOf<UDamageType> DamageType;

	virtual void NotifyTick(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation) override;
	virtual void NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation, float TotalDuration) override;
};
