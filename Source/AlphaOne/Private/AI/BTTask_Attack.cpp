#include "AI/BTTask_Attack.h"
//UE4
#include "characters/CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"



UBTTask_Attack::UBTTask_Attack()
{
    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr) {
        return EBTNodeResult::Failed;
    }
    //call the attack function of the pawn
    ACharacterBase* Character = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());

    if (Character == nullptr) {
        return EBTNodeResult::Failed;
    }

    Character->Attack();

    return EBTNodeResult::Succeeded;
}

