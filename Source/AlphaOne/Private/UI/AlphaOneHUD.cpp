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

void AAlphaOneHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawHealth();
}

void AAlphaOneHUD::DrawHealth()
{
	ACharacterBase* Player = Cast<ACharacterBase>(GetOwningPawn());

    if (IsValid(Player)) {
        auto Attr = Player->GetAttributes();
        auto percent = Attr->GetHealth() / Attr->GetMaxHealth();
        HealthBar->SetPercent(percent);

        FString Text = FString::Printf(TEXT("%d/%d"), int(Attr->GetHealth()), int(Attr->GetMaxHealth()));
        HealthText->SetText(FText::FromString(Text));
    }
}
