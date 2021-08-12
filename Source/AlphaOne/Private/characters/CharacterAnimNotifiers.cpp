#include "characters/CharacterAnimNotifiers.h"
#include "characters/CharacterBase.h"


void UJumpEndNotify::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation)
{
    auto Player = Cast<ACharacterBase>(Mesh->GetOwner());
    if (Player) {
        Player->SetAction(EUnitActions::Jumping, false);
    }
}

