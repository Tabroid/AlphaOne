// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;
class UTextBlock;
class ATower;

UCLASS(Abstract)
class ALPHAONE_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwnerActor(ATower* Owner);

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	TWeakObjectPtr<ATower> OwnerActor;

	UPROPERTY (meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	UPROPERTY (meta = (BindWidget))
	UTextBlock* HealthPercentage = nullptr;
};



