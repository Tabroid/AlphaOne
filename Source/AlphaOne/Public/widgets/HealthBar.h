// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "characters/CharacterAttributes.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS(Abstract)
class ALPHAONE_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetAttributeSet(UCharacterAttributes* Attr) { AttributeSet = Attr; }

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	TWeakObjectPtr<UCharacterAttributes> AttributeSet;

	UPROPERTY (meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	UPROPERTY (meta = (BindWidget))
	UTextBlock* HealthPercentage = nullptr;
};



