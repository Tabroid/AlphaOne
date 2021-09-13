// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "UnitAttributes.h"
#include "abilities/AlphaOneAbilitySystem.h"
#include "GameFramework/Actor.h"
#include "BuildingBase.generated.h"

class UCapsuleComponent;
class UWidgetComponent;
class ACharacterBase;

UCLASS()
class ALPHAONE_API ABuildingBase : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABuildingBase();

    // Called every frame
    virtual void Tick(float DeltaTime) override;
    virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
    virtual void Die(float DamageAmount, const FDamageEvent& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

    // Implement IAbilitySystemInterface
    UAbilitySystemComponent* GetAbilitySystemComponent() const;

    UFUNCTION(BlueprintCallable)
    UCharacterAttributes* GetAttributes() { return AttributeSet; }

    UFUNCTION(BlueprintCallable)
    void SetAction(EUnitActions NewAction, bool State = true);

    UFUNCTION(BlueprintCallable)
    void SetStatus(EUnitStatuses NewStatus, bool State = true);

    UFUNCTION(BlueprintCallable)
    void SetType(EUnitTypes NewType);

    UFUNCTION(BlueprintCallable)
    FORCEINLINE bool CheckType(EUnitTypes Type) const { return (AttributeSet->Type == Type); }

    UFUNCTION(BlueprintCallable)
    FORCEINLINE bool CheckAction(EUnitActions Action) const { return static_cast<bool>(AttributeSet->Action & Action); }

    UFUNCTION(BlueprintCallable)
    FORCEINLINE bool CheckStatus(EUnitStatuses Status) const { return static_cast<bool>(AttributeSet->Status & Status); }

    UFUNCTION(BlueprintCallable)
    UFactionComponent* GetFactionComponent() { return FactionSystemComponent; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY()
    UCharacterAttributes* AttributeSet;

    UPROPERTY()
    UAlphaOneAbilitySystem* AbilitySystemComponent;

    //various component for assigning mesh later on
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCapsuleComponent*    CapsuleComp = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* BaseMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* AuraMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UWidgetComponent* HealthBarUI = nullptr;

    // A proxy to FactionSystem component
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction", meta = (AllowPrivateAccess = "true"))
    EUnitFactions DefaultFaction;

    UPROPERTY()
    UFactionComponent* FactionSystemComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
    FName UnitDataRowName = "Default_Building";

private:
    ACharacterBase* PlayerCharacter;
};
