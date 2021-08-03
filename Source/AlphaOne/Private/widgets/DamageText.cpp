// Fill out your copyright notice in the Description page of Project Settings.


#include "widgets/DamageText.h"
#include "Components/TextBlock.h"


void UDamageText::PopDamage(float Value, FVector2D Position, FLinearColor Color, float Life, float Distance)
{
    DamageText->SetColorAndOpacity(Color);
    DamageText->SetText(FText::AsNumber(int32(Value)));
    LifeSpan = Life;

    // find the final location
    float Sign = FMath::RandBool() ? 1.f : -1.f;
    Velocity.X = Sign * FMath::RandRange(Distance / 2.f, Distance) / Life;
    Velocity.Y = -FMath::RandRange(Distance, Distance * 2.f) / Life;
    Acceleration.X = 0.;
    Acceleration.Y = -2.f * Velocity.Y / Life;
    ViewPosition = Position;
    SetPositionInViewport(ViewPosition);
}

void UDamageText::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    Timer += InDeltaTime;
    FVector2D NewVelocity = Velocity + Acceleration*InDeltaTime;
    ViewPosition += (NewVelocity + Velocity)/2.*InDeltaTime;
    Velocity = NewVelocity;
    SetPositionInViewport(ViewPosition);
    DamageText->SetOpacity(std::max(1.0f - Timer / LifeSpan, 0.f));
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Opacity: %.02f!"), ColorAndOpacity.A));
}

