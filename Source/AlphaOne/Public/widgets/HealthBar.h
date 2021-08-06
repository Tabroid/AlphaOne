// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "HealthBar.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS(Abstract)
class ALPHAONE_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetAttributeSet(UCharacterAttributes* Attr);
	void SetOwningComponent(UWidgetComponent* Attr);
	void UpdatePercentage(float NewVal, float OldVal);

	virtual void BeginDestroy() override;

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	TWeakObjectPtr<UCharacterAttributes> AttributeSet;
	TWeakObjectPtr<UWidgetComponent> OwningComponent;

	UPROPERTY (meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	UPROPERTY (meta = (BindWidget))
	UTextBlock* HealthPercentage = nullptr;

	float FullHealthDuration = 0.f;
};
