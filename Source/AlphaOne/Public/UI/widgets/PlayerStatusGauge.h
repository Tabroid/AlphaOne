#pragma once

#include "AlphaOne.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "PlayerStatusGauge.generated.h"


UCLASS(Abstract)
class ALPHAONE_API UPlayerStatusGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindPlayerCharacter(class ACharacterBase* Character);
    void UnBindPlayerCharacter();

    void OnHealthChanged(float NewVal, float OldVal);
    void OnManaChanged(float NewVal, float OldVal);
    void OnAbsorptionChanged(float NewVal, float OldVal);

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UPROPERTY (meta = (BindWidget))
	class UProgressBar* HealthGauge = nullptr;

    UPROPERTY (meta = (BindWidget))
	class UProgressBar* ManaGauge = nullptr;

    UPROPERTY (meta = (BindWidget))
	class UProgressBar* AbsorptionGauge = nullptr;

private:
    ACharacterBase *PlayerCharacter = nullptr;

    bool bNeedsInit = false;
};
