
#include "weapons/RangedWeapon.h"
#include "characters/Archer.h"
#include "Kismet/GameplayStatics.h"


ARangedWeapon::ARangedWeapon()
{
}

void ARangedWeapon::AttachToCharacter(ACharacterBase* Character)
{
    Super::AttachToCharacter(Character);
}

void ARangedWeapon::DetachFromCharacter()
{
     Super::DetachFromCharacter();
}

bool ARangedWeapon::Attack()
{
    // can only attack when aiming
    if (!MyCharacter->CheckAction(EUnitActions::Aiming)) {
        return false;
    }
    return Super::Attack();
}

bool ARangedWeapon::ShootProjectile()
{
    FTransform SpawnTM(GetEmitterRotation(), GetEmitterLocation());
    auto Projectile = Cast<AProjectileBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTM));
    if (Projectile) {
        Projectile->SetInstigator(MyCharacter);
        Projectile->SetOwner(MyCharacter);
        Projectile->AddIgnoreActors({MyCharacter, this});
        UGameplayStatics::FinishSpawningActor(Projectile, SpawnTM);
        return true;
    }
    return false;
}