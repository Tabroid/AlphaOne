// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EItemAllowedInteractions: uint8
{
    NONE            = 0          UMETA(Hidden),
    Stackable       = 1 << 0     UMETA(DisplayName = "Stackable"),
    Consumable      = 1 << 1     UMETA(DisplayName = "Consumable"),
	Equipable		= 1 << 2	 UMETA(DisplayName = "Equipable"),
	Sellable		= 1 << 3	 UMETA(DisplayName = "Sellable"),
	Droppable		= 1 << 4	 UMETA(DisplayName = "Droppable"),
};
ENUM_CLASS_FLAGS(EItemAllowedInteractions);

UCLASS()
class ALPHAONE_API UItemBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Constructor
	UItemBase()
		: Price(0)
		, MaxCount(1)
		, MaxLevel(1)
	{}

	// Type of this item, set in native parent class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FPrimaryAssetType ItemType;

	// User-visible short name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	// User-visible long description
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription;

	// Icon to display
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FSlateBrush ItemIcon;

	// Price in game
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Price;

	// Maximum number of instances that can be in inventory at once, <= 0 means infinite
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Max")
	int32 MaxCount;

	// Maximum level this item can be, <= 0 means infinite
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Max")
	int32 MaxLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item",  meta = (Bitmask, BitmaskEnum = EUnitActions))
	EItemAllowedInteractions AllowedInteractions;

	// Returns the logical name, equivalent to the primary asset id
	UFUNCTION(BlueprintCallable, Category = "Item")
	FString GetIdentifierString() const;

	// Overridden to use saved type
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckAllowedInteraction(EItemAllowedInteractions Interaction) { return static_cast<bool>(AllowedInteractions & Interaction); }

	UFUNCTION(BlueprintCallable)
	void SetAllowedInteraction(EItemAllowedInteractions Interaction, bool NewState = true);

};
