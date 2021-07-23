// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HealthBar.h"
#include "Components/ProgressBar.h"
#include "building/Tower.h"
#include "Components/TextBlock.h"

void UHealthBar::SetOwnerActor(ATower* Owner) 
{
    OwnerActor = Owner;
}

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) 
{
    Super::NativeTick(MyGeometry, InDeltaTime);

	if (!OwnerActor.IsValid()) return;

    HealthBar->SetPercent(OwnerActor->GetHealthPercentage()/100.0f);

    FString PrintText = FString::Printf( TEXT("%0.1f %%"), OwnerActor->GetHealthPercentage());
    HealthPercentage->SetText(FText::FromString(PrintText));
}

