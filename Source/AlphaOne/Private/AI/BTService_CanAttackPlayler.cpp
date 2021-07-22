// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CanAttackPlayler.h"

UBTService_CanAttackPlayler::UBTService_CanAttackPlayler() 
{
	NodeName = TEXT("Can Attack Player");
}

void UBTService_CanAttackPlayler::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if (OwnerComp.GetAIOwner() == nullptr) return;

	//check if the enemy can attack the player, if yes, set the blackboard key to the player
	//if not, clear the blackboard key

}

