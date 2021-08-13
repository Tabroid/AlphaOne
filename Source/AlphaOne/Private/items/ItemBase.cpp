// Fill out your copyright notice in the Description page of Project Settings.


#include "items/ItemBase.h"


FString UItemBase::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UItemBase::GetPrimaryAssetId() const
{
	// This is a DataAsset and not a blueprint so we can just use the raw FName
	// For blueprints you need to handle stripping the _C suffix
	return FPrimaryAssetId(ItemType, GetFName());
}

void UItemBase::SetAllowedInteraction(EItemAllowedInteractions Interaction, bool NewState)
{
	if (NewState) {
		AllowedInteractions |= Interaction;
	} else {
		AllowedInteractions &= ~Interaction;
	}
}
