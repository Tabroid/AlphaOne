#pragma once

#include "AlphaOne.h"
#include "weapons/WeaponBase.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MeleeSweeper.generated.h"


UCLASS()
class ALPHAONE_API UMeleeSweeper : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyTick(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation) override;
	virtual void NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation, float TotalDuration) override;
};
