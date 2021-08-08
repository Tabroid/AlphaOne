#include "UI/AlphaOneHUD.h"
#include "UnitAttributes.h"
#include "characters/CharacterBase.h"
#include "controllers/AlphaOnePlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


AAlphaOneHUD::AAlphaOneHUD(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void AAlphaOneHUD::BeginPlay()
{
    if (StatusGaugeClass) {
        StatusGaugeWidget = CreateWidget<UPlayerStatusGauge>(GetWorld(), StatusGaugeClass);
        if (StatusGaugeWidget) {
            StatusGaugeWidget->AddToViewport();
        }
    }
}

void AAlphaOneHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AAlphaOneHUD::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}