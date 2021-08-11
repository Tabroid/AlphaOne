#pragma once

#include "AlphaOne.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "weapons/WeaponBase.h"
#include "WeaponAnimNotifiers.generated.h"


UCLASS()
class ALPHAONE_API USweepBeginNotify : public UAnimNotify
{
    GENERATED_BODY()
public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS()
class ALPHAONE_API USweepEndNotify : public UAnimNotify
{
    GENERATED_BODY()
public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS()
class ALPHAONE_API UComboReadyNotify : public UAnimNotify
{
    GENERATED_BODY()
public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS()
class ALPHAONE_API UComboResetNotify : public UAnimNotify
{
    GENERATED_BODY()
public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS()
class ALPHAONE_API UProjectileFireNotify : public UAnimNotify
{
    GENERATED_BODY()
public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
