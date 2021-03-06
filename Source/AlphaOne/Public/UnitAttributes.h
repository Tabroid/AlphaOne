#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "UnitAttributes.generated.h"

// regen is pretty cost because it triggers all delegates
#define ALPHAONE_UNIT_REGEN_TICK 2.0f

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
    Aiming			= 1 << 4     UMETA(DisplayName = "Aiming"),
    Jumping			= 1 << 5     UMETA(DisplayName = "Jumping"),
    Turning         = 1 << 6     UMETA(DisplayName = "Turning"),
};
ENUM_CLASS_FLAGS(EUnitActions);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EUnitStatuses: uint8
{
    NONE     = 0          UMETA(Hidden),
    Dead     = 1 << 0     UMETA(DisplayName = "Dead"),
};
ENUM_CLASS_FLAGS(EUnitStatuses);

// This provides the interface to DataTable
USTRUCT()
struct FUnitDataRow : public FTableRowBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere) int32 Level = 1;
    UPROPERTY(EditAnywhere) float RotationRate = 1.f;
    UPROPERTY(EditAnywhere) float JogSpeed = 1.f;
    UPROPERTY(EditAnywhere) float SprintSpeed = 1.f;

    UPROPERTY(EditAnywhere) float Health = 100.f;
    UPROPERTY(EditAnywhere) float MaxHealth = 100.f;
    UPROPERTY(EditAnywhere) float HealthRegen = 0.f;
    UPROPERTY(EditAnywhere) float Absorption = 0.f;
    UPROPERTY(EditAnywhere) float MaxAbsorption = 0.f;
    UPROPERTY(EditAnywhere) float AbsorptionRegen = 0.f;

    UPROPERTY(EditAnywhere) float Mana = 100.f;
    UPROPERTY(EditAnywhere) float MaxMana = 100.f;
    UPROPERTY(EditAnywhere) float ManaRegen = 2.f;

    UPROPERTY(EditAnywhere) float AttackPower = 5.f;
    UPROPERTY(EditAnywhere) float CriticalChance = 0.05f;
    UPROPERTY(EditAnywhere) float CriticalDamage = 1.f;
    UPROPERTY(EditAnywhere) float ArmorPenetration = 0.f;
    UPROPERTY(EditAnywhere) float DamageAmplification = 0.f;
    UPROPERTY(EditAnywhere) float HitRate = 1.f;
    UPROPERTY(EditAnywhere) float AttackSpeed = 1.f;

    UPROPERTY(EditAnywhere) float Armor = 0.f;
    UPROPERTY(EditAnywhere) float DamageReduction = 0.f;
    UPROPERTY(EditAnywhere) float DodgeRate = 0.f;
};

using FOnAttributeChanged = TDelegate<void(float NewVal, float OldVal)>;

// helper macro for adding an attribute
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

// helper macro for adding an attribute with changed
#define ADD_ATTRIBUTE_DELEGATED(ClassName, PropertyName, Category) \
    public: \
    UPROPERTY(BlueprintReadOnly, Category, ReplicatedUsing=OnRep_##PropertyName) \
    FGameplayAttributeData PropertyName; \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    FORCEINLINE void Set##PropertyName(float NewVal) { \
        UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent(); \
        if (ensure(AbilityComp)) { \
            float OldVal = PropertyName.GetBaseValue(); \
            AbilityComp->SetNumericAttributeBase(Get##PropertyName##Attribute(), NewVal); \
            Execute##PropertyName##ChangedDelegates(NewVal, OldVal); \
        } \
    } \
    FORCEINLINE void Init##PropertyName(float NewVal) { \
        float OldVal = PropertyName.GetCurrentValue(); \
        PropertyName.SetBaseValue(NewVal); \
        PropertyName.SetCurrentValue(NewVal); \
        Execute##PropertyName##ChangedDelegates(NewVal, OldVal); \
    } \
     FORCEINLINE void Add##PropertyName##ChangedDelegate(FOnAttributeChanged NewDelegate, FString KeyVal) { \
        PropertyName##ChangedDelegates.Add(KeyVal, NewDelegate); \
    } \
    FORCEINLINE int32 Remove##PropertyName##ChangedDelegate(FString KeyVal) { \
        return PropertyName##ChangedDelegates.Remove(KeyVal); \
    } \
    FORCEINLINE void Reset##PropertyName##ChangedDelegate() { \
        PropertyName##ChangedDelegates.Empty(); \
    } \
    FORCEINLINE void Execute##PropertyName##ChangedDelegates(float NewVal, float OldVal) { \
        for (auto& [KeyVal, Delegate] : PropertyName##ChangedDelegates) { \
            if (Delegate.IsBound()) { Delegate.Execute(NewVal, OldVal); } \
        } \
    } \
    protected: \
    TMap<FString, FOnAttributeChanged> PropertyName##ChangedDelegates; \
    UFUNCTION() \
    virtual void OnRep_##PropertyName(const FGameplayAttributeData& OldValue) { \
        GAMEPLAYATTRIBUTE_REPNOTIFY(ClassName, PropertyName, OldValue); \
    } \


// This holds all of the attributes used by abilities, it instantiates a copy of this on every character
UCLASS()
class ALPHAONE_API UCharacterAttributes : public UAttributeSet
{
    GENERATED_BODY()

    ADD_ATTRIBUTE(UCharacterAttributes, RotationRate, "Basic")
    ADD_ATTRIBUTE(UCharacterAttributes, JogSpeed, "Basic")
    ADD_ATTRIBUTE(UCharacterAttributes, SprintSpeed, "Basic")

    ADD_ATTRIBUTE_DELEGATED(UCharacterAttributes, Health, "Health")
    ADD_ATTRIBUTE(UCharacterAttributes, MaxHealth, "Health")
    ADD_ATTRIBUTE(UCharacterAttributes, HealthRegen, "Health")
    ADD_ATTRIBUTE_DELEGATED(UCharacterAttributes, Absorption, "Health")
    ADD_ATTRIBUTE(UCharacterAttributes, MaxAbsorption, "Health")
    ADD_ATTRIBUTE(UCharacterAttributes, AbsorptionRegen, "Health")

    ADD_ATTRIBUTE_DELEGATED(UCharacterAttributes, Mana, "Mana")
    ADD_ATTRIBUTE(UCharacterAttributes, MaxMana, "Mana")
    ADD_ATTRIBUTE(UCharacterAttributes, ManaRegen, "Mana")

    ADD_ATTRIBUTE(UCharacterAttributes, AttackPower, "Damage")
    ADD_ATTRIBUTE(UCharacterAttributes, CriticalChance, "Damage")
    ADD_ATTRIBUTE(UCharacterAttributes, CriticalDamage, "Damage")
    ADD_ATTRIBUTE(UCharacterAttributes, ArmorPenetration, "Damage")
    ADD_ATTRIBUTE(UCharacterAttributes, DamageAmplification, "Damage")
    ADD_ATTRIBUTE(UCharacterAttributes, HitRate, "Damage")
    ADD_ATTRIBUTE(UCharacterAttributes, AttackSpeed, "Damage")

    ADD_ATTRIBUTE(UCharacterAttributes, Armor, "Defense")
    ADD_ATTRIBUTE(UCharacterAttributes, DamageReduction, "Defense")
    ADD_ATTRIBUTE(UCharacterAttributes, DodgeRate, "Defense")

public:
    // Constructor and overrides
    UCharacterAttributes();

    // virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    // status change by time
    virtual void RegenOverTime(float DeltaTime);
    // @TODO: add effects
    virtual bool InitFromMetaDataTable(const UDataTable* DataTable, FName RowName);
    virtual void InitFromMetaDataTable(const UDataTable* DataTable) override {
        InitFromMetaDataTable(DataTable, Name);
    }

    // @TODO level may affect other attributes
    int32 GetLevel() const { return Level; }
    virtual bool SetLevel(int32 NewValue);
    virtual void InitLevel(int32 NewValue);
    virtual bool LevelUp();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EUnitActions))
    EUnitActions Action;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EUnitStatus))
    EUnitStatuses Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EUnitTypes Type;

    UPROPERTY(BlueprintReadOnly, Category = "Basic") //, ReplicatedUsing = OnRep_Level)
    int32 Level;

    UPROPERTY(BlueprintReadOnly, Category = "Basic") //, ReplicatedUsing = OnRep_Level)
    FName Name = "Default";

protected:
    // Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
    // (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
    void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute,
                                     float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
    float RegenTimer;
};
