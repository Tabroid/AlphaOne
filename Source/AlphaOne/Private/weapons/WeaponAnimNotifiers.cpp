#include "weapons/WeaponAnimNotifiers.h"
#include "characters/CharacterBase.h"


void USweepBeginNotify::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation)
{
    auto Player = Cast<ACharacterBase>(Mesh->GetOwner());
    if (Player && Player->GetCurrentWeapon()) {
        Player->GetCurrentWeapon()->OnSweepBegin();
    }
}


void USweepEndNotify::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation)
{
    auto Player = Cast<ACharacterBase>(Mesh->GetOwner());
    if (Player && Player->GetCurrentWeapon()) {
        Player->GetCurrentWeapon()->OnSweepEnd();
    }
}

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

