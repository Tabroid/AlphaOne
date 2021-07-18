// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// ----------------------------------------------------------------------------------------------------------------
// This header is for Ability-specific structures and enums that are shared across a project
// Every game will probably need a file like this to handle their extensions to the system
// This file is a good place for subclasses of FGameplayEffectContext and FGameplayAbilityTargetData
// ----------------------------------------------------------------------------------------------------------------

#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameplayEffectsContainer.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
// class UTargetType;


/**
 * Struct defining a list of gameplay effects, a tag, and targeting info
 * These containers are defined statically in blueprints or assets and then turn into Specs at runtime
 */
USTRUCT(BlueprintType)
struct FGameplayEffectContainer
{
	GENERATED_BODY()

public:
	FGameplayEffectContainer() {}

	/** Sets the way that targeting happens */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	// TSubclassOf<UTargetType> TargetType;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;
};

/** A "processed" version of RPGGameplayEffectContainer that can be passed around and eventually applied */
USTRUCT(BlueprintType)
struct FGameplayEffectContainerSpec
{
	GENERATED_BODY()

public:
	FRPGGameplayEffectContainerSpec() {}

	/** Computed target data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	FGameplayAbilityTargetDataHandle TargetData;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

	/** Returns true if this has any valid effect specs */
	bool HasValidEffects() const {
		return TargetGameplayEffectSpecs.Num() > 0;
	}

	/** Returns true if this has any valid targets */
	bool HasValidTargets() const {
		return TargetData.Num() > 0;
	}

	/** Adds new targets to target data */
	void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors) {
		for (const FHitResult& HitResult : HitResults) {
			FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
			TargetData.Add(NewData);
		}

		if (TargetActors.Num() > 0) {
			FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
			NewData->TargetActorArray.Append(TargetActors);
			TargetData.Add(NewData);
		}
	}
};
