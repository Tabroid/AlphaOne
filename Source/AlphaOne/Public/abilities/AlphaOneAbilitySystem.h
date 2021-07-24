#pragma once

#include "AlphaOne.h"
#include "AbilitySystemComponent.h"
#include "abilities/AlphaOneAbilityType.h"
#include "AlphaOneAbilitySystem.generated.h"

class UAlphaOneAbility;

/**
 * Subclass of ability system component with game-specific data
 * Most games will need to make a game-specific subclass to provide utility functions
 */
UCLASS()
class ALPHAONE_API UAlphaOneAbilitySystem : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Constructors and overrides
	UAlphaOneAbilitySystem();

	/** Returns a list of currently active ability instances that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UAlphaOneAbility*>& ActiveAbilities);

	/** Returns the default level used for ability activations, derived from the character */
	int32 GetDefaultAbilityLevel() const;

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static UAlphaOneAbilitySystem* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

};