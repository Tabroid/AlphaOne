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
    virtual bool ShootProjectile() override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetEmitterLocation() const { return GetSocketLocation(ProjectileSocketEmitter); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FRotator GetEmitterRotation() const { return GetSocketRotation(ProjectileSocketEmitter); }

	//projectile of this pawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
    FName ProjectileSocketEmitter = "BowEmitterSocket";
};
