// An instance class that manages "singletons"

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AlphaOneBattle.h"
#include "AlphaOneInstance.generated.h"


UCLASS()
class ALPHAONE_API UAlphaOneInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UAlphaOneInstance();

	// Accessor for C++ & BP. Should be self-explanatory
	UFUNCTION(BlueprintPure, Category = "Persistence", meta = (DisplayName = "Battle"))
	UAlphaOneBattle *Battle() const { return BattleInstance; };

protected:
 	// This is where we will clean up, as the game is shut dow
	virtual void Shutdown() override;

private:
	// Needs to be an UPROPERTY(), so the GC does its thing
	// Transient, so it doesn't get serialized itself. Will be nulled when the
	// game starts. Initialization happens later by hand.
	UPROPERTY(Transient)
	UAlphaOneBattle *BattleInstance;
};
