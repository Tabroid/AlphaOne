
#include "weapons/RangedWeapon.h"
#include "characters/Archer.h"


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
    /*
    if (!MyCharacter->CheckAction(EUnitActions::Aiming)) {
        return false;
    }
    */
    return Super::Attack();
}

bool ARangedWeapon::ShootProjectile()
{
    if (!ProjectileClass) {
        return false;
    }
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Shoot Projectile!"));
	auto Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, GetEmitterLocation(), GetEmitterRotation());
	//set the owner
	Projectile->SetOwner(MyCharacter);
    Projectile->AddIgnoreActors({MyCharacter, this});

	return true;
}