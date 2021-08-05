// A general UI for playing
#pragma once

#include "AlphaOne.h"
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
	virtual void DrawHealth();

protected:
	UPROPERTY (meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	UPROPERTY (meta = (BindWidget))
	UTextBlock* HealthText = nullptr;
};