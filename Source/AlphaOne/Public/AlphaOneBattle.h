#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "abilities/AlphaOneAbilitySystem.h"
#include "AlphaOneBattle.generated.h"

class UCharacterAttributes;

// helper inline
template<class AttrSetClass = UCharacterAttributes>
static inline const AttrSetClass* _Attr(const AActor* Actor) {
	auto AbilityComp = Actor->FindComponentByClass<UAlphaOneAbilitySystem>();
	return AbilityComp ? AbilityComp->GetSet<AttrSetClass>() : nullptr;
}

UCLASS()
class ALPHAONE_API UAlphaOneBattle : public UObject
{
	GENERATED_BODY()

public:
	UAlphaOneBattle();
	~UAlphaOneBattle();

	float CalcDamage(float DamageBase, const UCharacterAttributes* CauserAttr, const UCharacterAttributes* TakerAttr) const;

	// helper interfaces
	inline float CalcDamage(float DamageBase, const UCharacterAttributes* CauserAttr, const AActor* Taker) const {
		return CalcDamage(DamageBase, CauserAttr, _Attr<>(Taker));
	}
	inline float CalcDamage(float DamageBase, const AActor* Causer, const UCharacterAttributes* TakerAttr) const {
		return CalcDamage(DamageBase, _Attr<>(Causer), TakerAttr);
	}
	inline float CalcDamage(float DamageBase, const AActor* Causer, const AActor* Taker) const {
		return CalcDamage(DamageBase, _Attr<>(Causer), _Attr<>(Taker));
	}
};
