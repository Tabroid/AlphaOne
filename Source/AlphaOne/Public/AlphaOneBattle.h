#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "abilities/AlphaOneAbilitySystem.h"
#include "AlphaOneBattle.generated.h"

class UCharacterAttributes;
class UDamageText;

UCLASS()
class ALPHAONE_API UAlphaOneBattle : public UObject
{
	GENERATED_BODY()

public:
	UAlphaOneBattle();
	~UAlphaOneBattle();

	float InflictDamage(float DamageBase, AActor* Causer, AActor* Taker) const;

protected:
	void PopDamageText(float Damage, AActor* Causer, AActor* Taker) const;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageText> DamageTextRef;
};
