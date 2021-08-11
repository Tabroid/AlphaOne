
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

bool ARangedWeapon::ShootProjectile()
{
    // TODO move the spawn component to ranged weapon class
    auto Archer = Cast<AArcher>(MyCharacter);
    if (!Projectile || !Archer) {
        return false;
    }
    auto ProjectileSpawn = Archer->GetProjectileSpawnComponent();
	FVector SpawnLocation = ProjectileSpawn->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawn->GetComponentRotation();
	AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(Projectile, SpawnLocation, SpawnRotation);
	//set the owner
	TempProjectile->SetOwner(Archer);
	return true;
}