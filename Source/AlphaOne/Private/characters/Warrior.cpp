#include "characters/Warrior.h"
#include "GameFramework/CharacterMovementComponent.h"


AWarrior::AWarrior()
{
    SetType(EUnitTypes::Hero);
}


bool AWarrior::Attack()
{
    /*
    if (Super::Attack()) {
        GetCharacterMovement()->StopMovementImmediately();
        GetCharacterMovement()->DisableMovement();
        return true;
    }
    return false;
    */
    return Super::Attack();
}

void AWarrior::OnAttackEnd(bool Interrupted)
{
    // GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    Super::OnAttackEnd(Interrupted);
}
