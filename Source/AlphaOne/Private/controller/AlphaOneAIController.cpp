// Fill out your copyright notice in the Description page of Project Settings.


#include "controller/AlphaOneAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AAlphaOneAIController::BeginPlay() 
{
    Super::BeginPlay();

    if (AIBehavior) {
        RunBehaviorTree(AIBehavior);
        //TODO: need to set the TargetLocation here, that's where this enemy is going to go
        //GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLocation"), );
    }
}

bool AAlphaOneAIController::IsDead() const
{
    //for now
	return false;
}



