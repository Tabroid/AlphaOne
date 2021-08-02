#include "weapons/MeleeSweeper.h"
#include "characters/CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"


void UMeleeSweeper::NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation, float TotalDuration)
{
    auto Player = Cast<ACharacterBase>(Mesh->GetOwner());
    if (Player && Player->GetCurrentWeapon()) {
        Player->GetCurrentWeapon()->OnSweepBegin();
    }
}

void UMeleeSweeper::NotifyTick(USkeletalMeshComponent* /*Mesh*/, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
}

void UMeleeSweeper::NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation)
{
    auto Player = Cast<ACharacterBase>(Mesh->GetOwner());
    if (Player && Player->GetCurrentWeapon()) {
        Player->GetCurrentWeapon()->OnSweepEnd();
    }
}