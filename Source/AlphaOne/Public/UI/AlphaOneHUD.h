// A general UI for playing
#pragma once

#include "AlphaOne.h"
#include "UI/widgets/PlayerStatusGauge.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "AlphaOneHUD.generated.h"


class UProgressBar;
class UTextBlock;

UCLASS()
class ALPHAONE_API AAlphaOneHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

public:
	virtual void DrawHUD() override;
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<UPlayerStatusGauge> StatusGaugeClass;

private:
	UPlayerStatusGauge *StatusGaugeWidget;
};