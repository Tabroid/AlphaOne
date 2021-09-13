// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AlphaOnePlayerController.generated.h"


UCLASS()
class ALPHAONE_API AAlphaOnePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void OnPossess(APawn *InPawn) override;
    virtual void OnUnPossess() override;
};
