#include "weapons/MeleeSweeper.h"
#include "characters/CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"


void UMeleeSweeper::NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation, float TotalDuration)
{
    auto Player = Cast<ACharacterBase>(Mesh->GetOwner());
    if (Player && Player->GetCurrentWeapon()) {
        Weapon = Player->GetCurrentWeapon();
        IgnoreActors = { Cast<AActor>(Player) };
        SocketLocations_Prev.Reset(0);
        for (auto &[name, radius] : Weapon->GetCollisionSockets()) {
            SocketLocations_Prev.Add(Weapon->GetSocketLocation(name));
        }
    }
}

void UMeleeSweeper::NotifyTick(USkeletalMeshComponent* /*Mesh*/, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    if ((Weapon == nullptr) || Weapon->IsPendingKill()) {
        return;
    }

    for (int32 i = 0; i < Weapon->GetCollisionSockets().Num(); ++i) {
        auto &[name, radius] = Weapon->GetCollisionSockets()[i];
        // find collisions
        UKismetSystemLibrary::SphereTraceMulti(GetWorld(),
                                               Weapon->GetSocketLocation(name),
                                               SocketLocations_Prev[i],
                                               radius,
                                               ETraceTypeQuery::TraceTypeQuery4,
                                               false,
                                               IgnoreActors,
                                               EDrawDebugTrace::ForDuration,
                                               HitResult,
                                               true,
                                               FLinearColor::Red,
                                               FLinearColor::Green,
                                               10);
        for (auto Hit : HitResult) {
            auto Actor = Hit.GetActor();
            if (!HitActors.Contains(Actor)) {
                HitActors.Add(Actor);
                FPointDamageEvent DamageEvent(Weapon->GetDamage(), Hit, Hit.Normal, DamageType);
                auto Player = Cast<ACharacterBase>(Weapon->GetCharacter());
                Actor->TakeDamage(Weapon->GetDamage(), DamageEvent, Player->GetController(), Player);
            }
        }
    }
}

void UMeleeSweeper::NotifyEnd(USkeletalMeshComponent* /*Mesh*/, UAnimSequenceBase* /*Animation*/)
{
    HitActors.Empty();
    Weapon = nullptr;
}
