#include "weapons/MeleeSweeper.h"
#include "characters/CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"


void UMeleeSweeper::NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation, float TotalDuration)
{
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NOTIFY BEGIN!"));
    auto Player = Cast<ACharacterBase>(Mesh->GetOwner());
    if (Player && Player->GetCurrentWeapon()) {
        // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("GOT WEAPON!"));
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
        auto SocketLocation = Weapon->GetSocketLocation(name);
        // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("SWEEP: (%.1f, %.1f, %.1f) -> (%.1f, %.1f, %.1f)!"),
        //    SocketLocations_Prev[i].X, SocketLocations_Prev[i].Y, SocketLocations_Prev[i].Z, SocketLocation.X, SocketLocation.Y, SocketLocation.Z));
        UKismetSystemLibrary::SphereTraceMulti(Weapon->GetWorld(),
                                               SocketLocation,
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
        // update previous location
        SocketLocations_Prev[i] = SocketLocation;
    }
}

void UMeleeSweeper::NotifyEnd(USkeletalMeshComponent* /*Mesh*/, UAnimSequenceBase* /*Animation*/)
{
    HitActors.Empty();
    Weapon = nullptr;
}
