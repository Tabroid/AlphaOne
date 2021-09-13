#include "abilities/AlphaOneAbilitySystem.h"
#include "abilities/AlphaOneAbility.h"
#include "AbilitySystemGlobals.h"

UAlphaOneAbilitySystem::UAlphaOneAbilitySystem() {}

void UAlphaOneAbilitySystem::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UAlphaOneAbility*>& ActiveAbilities)
{
    TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
    GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

    // Iterate the list of all ability specs
    for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
    {
        // Iterate all instances on this ability spec
        TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

        for (UGameplayAbility* ActiveAbility : AbilityInstances)
        {
            ActiveAbilities.Add(Cast<UAlphaOneAbility>(ActiveAbility));
        }
    }
}

int32 UAlphaOneAbilitySystem::GetDefaultAbilityLevel() const
{
    auto attr = GetSet<UCharacterAttributes>();
    if (attr) {
        // @TODO: use another attribute for it
        return attr->GetLevel();
    }
    return 1;
}

UAlphaOneAbilitySystem* UAlphaOneAbilitySystem::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
    return Cast<UAlphaOneAbilitySystem>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}
