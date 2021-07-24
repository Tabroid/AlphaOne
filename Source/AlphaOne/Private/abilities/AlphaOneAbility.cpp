// Copyright Epic Games, Inc. All Rights Reserved.

#include "abilities/AlphaOneAbility.h"
#include "abilities/AlphaOneAbilitySystem.h"
#include "abilities/TargetType.h"
#include "characters/CharacterBase.h"

UAlphaOneAbility::UAlphaOneAbility() {}

FGameplayEffectContainerSpec UAlphaOneAbility::MakeEffectContainerSpecFromContainer(const FGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	// First figure out our actor info
	FGameplayEffectContainerSpec ReturnSpec;
	AActor* OwningActor = GetOwningActorFromActorInfo();
	ACharacterBase* OwningCharacter = Cast<ACharacterBase>(OwningActor);
	UAbilitySystemComponent* OwningASC = UAlphaOneAbilitySystem::GetAbilitySystemComponentFromActor(OwningActor);

	if (OwningASC) {
		// If we have a target type, run the targeting logic. This is optional, targets can be added later
		if (Container.TargetType.Get()) {
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			const UTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
			ReturnSpec.AddTargets(HitResults, TargetActors);
		}

		// If we don't have an override level, use the default on the ability itself
		if (OverrideGameplayLevel == INDEX_NONE) {
			OverrideGameplayLevel = OverrideGameplayLevel = this->GetAbilityLevel(); //OwningASC->GetDefaultAbilityLevel();
		}

		// Build GameplayEffectSpecs for each applied effect
		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses) {
			ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
		}
	}
	return ReturnSpec;
}

FGameplayEffectContainerSpec UAlphaOneAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

	if (FoundContainer)
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> UAlphaOneAbility::ApplyEffectContainerSpec(const FGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

TArray<FActiveGameplayEffectHandle> UAlphaOneAbility::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	return ApplyEffectContainerSpec(Spec);
}
