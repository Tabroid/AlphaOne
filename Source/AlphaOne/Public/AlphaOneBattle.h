#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "abilities/AlphaOneAbilitySystem.h"
#include "AlphaOneBattle.generated.h"

class UCharacterAttributes;
class UDamageText;


UENUM(BlueprintType)
enum class EDamageTypes: uint8
{
    Hit 	  = 0   UMETA(DisplayName = "Hit"),
	Critical  = 1   UMETA(DisplayName = "Critical"),
    Missed    = 2   UMETA(DisplayName = "Missed"),
	Dodged	  = 3   UMETA(DisplayName = "Dodged"),
	Max_Types 		UMETA(Hidden),
};

USTRUCT()
struct FAttackResult
{
    GENERATED_USTRUCT_BODY()
	float FinalDamage = 0.f;
	EDamageTypes DamageType = EDamageTypes::Hit;
};


UCLASS()
class ALPHAONE_API UAlphaOneBattle : public UObject
{
	GENERATED_BODY()

public:
	UAlphaOneBattle();
	~UAlphaOneBattle();

	FAttackResult InflictDamage(float DamageBase, const FDamageEvent& DamageEvent, AActor* Causer, AActor* Taker) const;

protected:
	void PopDamageText(const FAttackResult& Result, const AActor* Causer, const AActor* Taker) const;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageText> DamageTextRef;
};
