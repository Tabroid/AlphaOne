// Fill out your copyright notice in the Description page of Project Settings.

#include "controller/AlphaOneAIController.h"
#include "characters/CharacterBase.h"
#include "buildings/Tower.h"
// UE4
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AAlphaOneAIController::BeginPlay()
{
    Super::BeginPlay();
    ACharacterBase* ThisCharacter = Cast<ACharacterBase>(GetCharacter());

    if (AIBehavior && ThisCharacter) {
        RunBehaviorTree(AIBehavior);

        // Set the target location to the nearest tower
        ATower* NearestTower = nullptr;
        TArray<AActor*> AllTower;
        // TODO, implement view distance
        float Distance = 1.e9;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), AllTower);

        for (auto Tower : AllTower) {
            ATower* CurrentTower = Cast<ATower>(Tower);
            if (!CurrentTower || !CurrentTower->CanBeDamaged() || CurrentTower->GetStatus() == EUnitStatuses::Dead) {
                continue;
            }

            float CurrentDistance = FVector::Dist(CurrentTower->GetActorLocation(), ThisCharacter->GetActorLocation());
            if (CurrentDistance < Distance) {
                Distance = CurrentDistance;
                NearestTower = CurrentTower;
            }
        }

        if (NearestTower) {
            GetBlackboardComponent()->SetValueAsObject(TEXT("TargetObject"), NearestTower);
        }
    }

}

void AAlphaOneAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AAlphaOneAIController::IsDead() const
{
    //Get the controlled pawn of this controller
    ACharacterBase* ControlledPawn = Cast<ACharacterBase>(GetPawn());

    if (!ControlledPawn || ControlledPawn->GetStatus() == EUnitStatuses::Dead) {
        return true;
    }

    return false;
}



