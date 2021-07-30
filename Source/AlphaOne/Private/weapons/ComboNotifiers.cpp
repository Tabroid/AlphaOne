#include "weapons/ComboNotifiers.h"
#include "characters/CharacterBase.h"


void UComboReadyNotify::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation)
{
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NOTIFY BEGIN!"));
    auto Player = Cast<ACharacterBase>(Mesh->GetOwner());
    if (Player) {
        Player->OnAttackEnd();
    }
}


void UComboResetNotify::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation)
{
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NOTIFY BEGIN!"));
    auto Player = Cast<ACharacterBase>(Mesh->GetOwner());
    if (Player) {
        Player->OnAttackEnd(true);
    }
}

