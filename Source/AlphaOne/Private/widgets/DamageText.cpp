// Fill out your copyright notice in the Description page of Project Settings.


#include "widgets/DamageText.h"
#include "Components/TextBlock.h"
#include "controllers/AlphaOnePlayerController.h"
#include "Kismet/GameplayStatics.h"


void UDamageText::PopText(const APlayerController* Player, const FText& Text, FVector Position, FLinearColor Color, bool Parabola, float Life, float Distance)
{
    DamageText->SetColorAndOpacity(Color);
    DamageText->SetText(Text);
    LifeSpan = Life;

    UGameplayStatics::ProjectWorldToScreen(Player, Position, ViewPosition, false);
    SetPositionInViewport(ViewPosition);

    // define the trace
    if (Parabola) {
        float Sign = FMath::RandBool() ? 1.f : -1.f;
        Velocity.X = Sign * FMath::RandRange(Distance / 2.f, Distance) / Life;
        Velocity.Y = -FMath::RandRange(Distance, Distance * 2.f) / Life;
        Acceleration.Y = -2.f * Velocity.Y / Life;
    } else {
        Velocity.Y = - Distance / Life;
        Acceleration.Y = - Velocity.Y / Life;
    }
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

