#pragma once

#include "AlphaOne.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "PlayerStatusGauge.generated.h"


UCLASS(Abstract)
class ALPHAONE_API UPlayerStatusGauge : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY (meta = (BindWidget))
	class UProgressBar* HealthGauge = nullptr;
};
