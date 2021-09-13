// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AlphaOneFaction.generated.h"


UENUM(BlueprintType)
enum class EUnitFactions: uint8
{
    FREE        = 0   UMETA(Hidden),
    Protagonist = 1   UMETA(DisplayName = "Protagonist"),
    Defenders   = 2   UMETA(DisplayName = "Defenders"),
    Intruders   = 3   UMETA(DisplayName = "Intruders"),
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALPHAONE_API UFactionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFactionComponent();

    UFUNCTION(BlueprintCallable)
    EUnitFactions GetFaction() const { return Faction; }

    UFUNCTION(BlueprintCallable)
    void SetFaction(EUnitFactions NewFaction) { Faction = NewFaction; }

    UFUNCTION(BlueprintCallable)
    virtual bool IsEnemy(const AActor *Target) const {
        if (!IsValid(Target)) { return false; }
        return IsEnemy(Target->FindComponentByClass<UFactionComponent>());
    }

    virtual bool IsEnemy(const UFactionComponent *TargetFactionComponent) const;

    // virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EUnitFactions Faction;
};