#include "abilities/AlphaOneAbilityType.h"
#include "abilities/TargetType.h"
#include "characters/CharacterBase.h"

UAlphaOneAbilityType::UAlphaOneAbilityType() {}

FGameplayEffectContainerSpec UAlphaOneAbilityType::MakeEffectContainerSpecFromContainer(const FGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
    // @TODO
    FGameplayEffectContainerSpec ReturnSpec;
    return ReturnSpec;
}

FGameplayEffectContainerSpec UAlphaOneAbilityType::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
    FGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

    if (FoundContainer)
    {
        return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
    }
    return FGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> UAlphaOneAbilityType::ApplyEffectContainerSpec(const FGameplayEffectContainerSpec& ContainerSpec)
{
    TArray<FActiveGameplayEffectHandle> AllEffects;

    // Iterate list of effect specs and apply them to their target data
    for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
    {
        AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
    }
    return AllEffects;
}

TArray<FActiveGameplayEffectHandle> UAlphaOneAbilityType::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
    FGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
    return ApplyEffectContainerSpec(Spec);
}
