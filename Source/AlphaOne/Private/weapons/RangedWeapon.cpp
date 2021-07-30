
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
    auto Archer = Cast<AArcher>(MyCharacter);
    if (!Projectile || !Archer) {
        return false;
    }

    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Spawn arrows"));
    //spawning the projectile at the spawn point
    auto ProjectileSpawn = Archer->GetProjectileSpawnComponent();
	FVector SpawnLocation = ProjectileSpawn->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawn->GetComponentRotation();
	AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(Projectile, SpawnLocation, SpawnRotation);
	//set the owner
	TempProjectile->SetOwner(Cast<AActor>(Archer));
	return true;
}