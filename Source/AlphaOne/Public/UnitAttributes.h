// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "UnitAttributes.generated.h"

// Uses macros from AttributeSet.h
#define ADD_ATTRIBUTE(ClassName, PropertyName, Category) \
	public: \
	UPROPERTY(BlueprintReadOnly, Category, ReplicatedUsing=OnRep_##PropertyName) \
	FGameplayAttributeData PropertyName; \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \
	protected: \
	UFUNCTION() \
	virtual void OnRep_##PropertyName(const FGameplayAttributeData& OldValue) { \
		GAMEPLAYATTRIBUTE_REPNOTIFY(ClassName, PropertyName, OldValue); \
	}

// enums for unit types/actions/statuses
UENUM(BlueprintType)
enum class EUnitTypes: uint8
{
    UNDEFINED = 0   UMETA(Hidden),
    Hero	  = 1   UMETA(DisplayName = "Hero"),
    Building  = 2   UMETA(DisplayName = "Building"),
    Mob       = 3   UMETA(DisplayName = "Mob"),
    Elite	  = 4   UMETA(DisplayName = "Elite"),
    Boss      = 5   UMETA(DisplayName = "Boss"),
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EUnitActions: uint8
{
    NONE            = 0          UMETA(Hidden),
    Attacking       = 1 << 0     UMETA(DisplayName = "Attacking"),
    Running         = 1 << 1     UMETA(DisplayName = "Running"),
    BeingAttacked   = 1 << 2     UMETA(DisplayName = "BeingAttacked"),
    Dying           = 1 << 3     UMETA(DisplayName = "Dying"),
};
ENUM_CLASS_FLAGS(EUnitActions);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EUnitStatuses: uint8
{
    NONE     = 0          UMETA(Hidden),
    Dead     = 1 << 0     UMETA(DisplayName = "Dead"),
};
ENUM_CLASS_FLAGS(EUnitStatuses);


/** This holds all of the attributes used by abilities, it instantiates a copy of this on every character */
UCLASS()
class ALPHAONE_API UCharacterAttributes : public UAttributeSet
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UCharacterAttributes();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// @TODO: add effects
	// virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// status change by time
	virtual void NaturalChange(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EUnitActions))
	EUnitActions Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EUnitStatus))
	EUnitStatuses Status;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUnitTypes Type;

	UPROPERTY(BlueprintReadOnly, Category = "Basic") //, ReplicatedUsing = OnRep_Level)
	int32 Level;
	// @TODO level may affect other attributes
	int32 GetLevel() const { return Level; }
	bool SetLevel(int32 NewValue) { if (Level == NewValue) { return false; } Level = NewValue; return true; }
	void InitLevel() { Level = 1; }

	ADD_ATTRIBUTE(UCharacterAttributes, MoveSpeed, "Basic")
	ADD_ATTRIBUTE(UCharacterAttributes, Health, "Health")
	ADD_ATTRIBUTE(UCharacterAttributes, MaxHealth, "Health")
	ADD_ATTRIBUTE(UCharacterAttributes, HealthRegen, "Health")
	ADD_ATTRIBUTE(UCharacterAttributes, Absorption, "Health")
	ADD_ATTRIBUTE(UCharacterAttributes, Mana, "Mana")
	ADD_ATTRIBUTE(UCharacterAttributes, MaxMana, "Mana")
	ADD_ATTRIBUTE(UCharacterAttributes, ManaRegen, "Mana")
	ADD_ATTRIBUTE(UCharacterAttributes, AttackPower, "Damage")
	ADD_ATTRIBUTE(UCharacterAttributes, CriticalChance, "Damage")
	ADD_ATTRIBUTE(UCharacterAttributes, CriticalDamage, "Damage")
	ADD_ATTRIBUTE(UCharacterAttributes, ArmorPenetration, "Damage")
	ADD_ATTRIBUTE(UCharacterAttributes, DamageAmplification, "Damage")
	ADD_ATTRIBUTE(UCharacterAttributes, HitRate, "Damage")
	ADD_ATTRIBUTE(UCharacterAttributes, AttackSpeed, "Damage")
	ADD_ATTRIBUTE(UCharacterAttributes, DefensePower, "Defense")
	ADD_ATTRIBUTE(UCharacterAttributes, DamageReduction, "Defense")
	ADD_ATTRIBUTE(UCharacterAttributes, DodgeRate, "Defense")


protected:
	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute,
									 float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
};
