// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CanAttackPlayler.generated.h"

/**
 * 
 */
UCLASS()
class ALPHAONE_API UBTService_CanAttackPlayler : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CanAttackPlayler();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};

