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
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Instantiate Gauge");
    if (StatusGaugeClass) {
        StatusGaugeWidget = CreateWidget<UPlayerStatusGauge>(GetWorld(), StatusGaugeClass);
        if (StatusGaugeWidget) {
            StatusGaugeWidget->AddToViewport();
            auto Player = Cast<ACharacterBase>(GetOwningPlayerController()->GetPawn());
            StatusGaugeWidget->BindPlayerCharacter(Player);
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

void AAlphaOneHUD::PlayerPossessed(ACharacterBase *Player)
{
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Bind Player");
    if (StatusGaugeWidget) {
        StatusGaugeWidget->BindPlayerCharacter(Player);
    }
}

void AAlphaOneHUD::PlayerUnPossessed()
{
    if (StatusGaugeWidget) {
        StatusGaugeWidget->UnBindPlayerCharacter();
    }
}