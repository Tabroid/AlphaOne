// A class for general ranged weapon, it generates projectile to hit target

#include "AlphaOne.h"
#include "weapons/WeaponBase.h"
#include "weapons/ProjectileBase.h"
#include "RangedWeapon.generated.h"

class ACharacterBase;

UCLASS()
class ALPHAONE_API ARangedWeapon : public AWeaponBase
{
    GENERATED_BODY()

public:
	ARangedWeapon();

    virtual void AttachToCharacter(ACharacterBase *Character) override;
    virtual void DetachFromCharacter() override;
    virtual bool Attack() override;

	//projectile of this pawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> Projectile;
};
