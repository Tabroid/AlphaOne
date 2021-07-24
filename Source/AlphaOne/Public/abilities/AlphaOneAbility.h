// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AlphaOne.h"
#include "Abilities/GameplayAbility.h"
#include "abilities/AlphaOneAbilityType.h"
#include "AlphaOneAbility.generated.h"

/**
 * Subclass of ability blueprint type with game-specific data
 * This class uses GameplayEffectContainers to allow easier execution of gameplay effects based on a triggering tag
 * Most games will need to implement a subclass to support their game-specific code
 */
UCLASS()
class ALPHAONE_API UAlphaOneAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UAlphaOneAbility();

	/** Map of gameplay tags to gameplay effect containers */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
	TMap<FGameplayTag, FGameplayEffectContainer> EffectContainerMap;

	/** Make gameplay effect container spec to be applied later, using the passed in container */
	UFUNCTION(BlueprintCallable, Category = Ability, meta=(AutoCreateRefTerm = "EventData"))
	virtual FGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** Search for and make a gameplay effect container spec to be applied later, from the EffectContainerMap */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** Applies a gameplay effect container spec that was previously created */
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FGameplayEffectContainerSpec& ContainerSpec);

	/** Applies a gameplay effect container, by creating and then applying the spec */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);
};