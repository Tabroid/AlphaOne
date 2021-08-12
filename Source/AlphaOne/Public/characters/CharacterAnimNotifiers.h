#pragma once

#include "AlphaOne.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CharacterAnimNotifiers.generated.h"


UCLASS()
class ALPHAONE_API UJumpEndNotify : public UAnimNotify
{
    GENERATED_BODY()
public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
