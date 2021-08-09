// Fill out your copyright notice in the Description page of Project Settings.


#include "controllers/AlphaOnePlayerController.h"
#include "characters/CharacterBase.h"
#include "UI/AlphaOneHUD.h"

void AAlphaOnePlayerController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    auto MainHUD = GetHUD<AAlphaOneHUD>();
    if (MainHUD) {
        MainHUD->PlayerPossessed(Cast<ACharacterBase>(InPawn));
    }
}

void AAlphaOnePlayerController::OnUnPossess()
{
    Super::OnUnPossess();

    auto MainHUD = GetHUD<AAlphaOneHUD>();
    if (MainHUD) {
        MainHUD->PlayerUnPossessed();
    }
}
