// Fill out your copyright notice in the Description page of Project Settings.

#include "characters/CharacterBase.h"
#include "controller/AlphaOneAIController.h"
#include "buildings/Tower.h"
//UE4
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AAlphaOneAIController::BeginPlay() 
{
    Super::BeginPlay();

    if (AIBehavior) {
        RunBehaviorTree(AIBehavior);
        
        //Set the target location to the nearest tower
        ATower* NearestTower = nullptr;
        TArray<AActor*> AllTower;
        float Distance = 1.e9;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), AllTower);

        for (auto Tower : AllTower){
            ATower* CurrentTower = Cast<ATower>(Tower);
            if (!CurrentTower || CurrentTower->GetStatus() == EUnitStatuses::Dead) continue;
            ACharacterBase* ControlledPawn = Cast<ACharacterBase>(GetPawn());
            if (!ControlledPawn) continue;
            float CurrentDistance = FVector::Dist(CurrentTower->GetActorLocation(), 
                                                  ControlledPawn->GetActorLocation());
            if (CurrentDistance < Distance) {
                Distance = CurrentDistance;
                NearestTower = CurrentTower;
            }
        }
        if (NearestTower){
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
	
    if (ControlledPawn != nullptr && ControlledPawn->GetStatus() == EUnitStatuses::Dead){
        return true;
    }

    return true;
}



