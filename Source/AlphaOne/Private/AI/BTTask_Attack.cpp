// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"


UBTTask_Attack::UBTTask_Attack() 
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if (OwnerComp.GetAIOwner() == nullptr){
        return EBTNodeResult::Failed;
    }
    //call the attack function of the pawn 
    /*AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (Character == nullptr){
        return EBTNodeResult::Failed;
    }

    Character->Fire();
    */
    return EBTNodeResult::Succeeded;
}

