// Copyright Epic Games, Inc. All Rights Reserved.

#include "abilities/TargetType.h"
#include "abilities/GameplayAbility.h"
#include "characters/CharacterBase.h"

void UTargetType::GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	return;
}

void UTargetType_UseOwner::GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(TargetingCharacter);
}

void UTargetType_UseEventData::GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target) {
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}