// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributes.generated.h"

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
	ADD_ATTRIBUTE(UCharacterAttributes, DefensePower, "Defense")
	ADD_ATTRIBUTE(UCharacterAttributes, DamageReduction, "Defense")
	ADD_ATTRIBUTE(UCharacterAttributes, DodgeRate, "Defense")
	ADD_ATTRIBUTE(UCharacterAttributes, MoveSpeed, "Speed")
	ADD_ATTRIBUTE(UCharacterAttributes, BaseMoveSpeed, "Speed")
	ADD_ATTRIBUTE(UCharacterAttributes, AttackSpeed, "Speed")


protected:
	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute,
									 float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
};