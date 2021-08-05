// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS(Abstract)
class ALPHAONE_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetAttributeSet(UCharacterAttributes* Attr);
	void UpdatePercentage(float NewVal, float OldVal);

	virtual void BeginDestroy() override;

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	TWeakObjectPtr<UCharacterAttributes> AttributeSet;

	UPROPERTY (meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	UPROPERTY (meta = (BindWidget))
	UTextBlock* HealthPercentage = nullptr;
};
