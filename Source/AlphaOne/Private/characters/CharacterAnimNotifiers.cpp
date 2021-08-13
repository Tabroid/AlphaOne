#include "characters/CharacterAnimNotifiers.h"
#include "characters/CharacterBase.h"


void UJumpEndNotify::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation)
{
    auto Character = Cast<ACharacterBase>(Mesh->GetOwner());
    if (Character) {
        Character->SetAction(EUnitActions::Jumping, false);
    }
}

void UTurningNotifyState::NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation, float TotalDuration)
{
    auto Character = Cast<ACharacterBase>(Mesh->GetOwner());
    if (Character) {
        Character->SetAction(EUnitActions::Turning);
    }
}

void UTurningNotifyState::NotifyTick(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    auto Actor = Mesh->GetOwner();
    Actor->SetActorRotation(Actor->GetActorRotation().Add(0.f, TurningSpeed*FrameDeltaTime, 0.f));
}

void UTurningNotifyState::NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation)
{
    auto Character = Cast<ACharacterBase>(Mesh->GetOwner());
    if (Character) {
        Character->SetAction(EUnitActions::Turning, false);
    }
}
