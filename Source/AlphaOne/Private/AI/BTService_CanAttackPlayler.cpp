// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "AI/BTService_CanAttackPlayler.h"

UBTService_CanAttackPlayler::UBTService_CanAttackPlayler()
{
	NodeName = TEXT("Can Attack Player");
}

void UBTService_CanAttackPlayler::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// check if the enemy can attack the player, if yes, set the blackboard key to the player
	// if not, clear the blackboard key
	if (OwnerComp.GetAIOwner() == nullptr) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!PlayerPawn || !ControlledPawn) return;

	float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerPawn->GetActorLocation());

	//FVector HeadVector = (Cast<ACharacter>(ControlledPawn)->GetMesh()->GetSocketRotation(TEXT("head"))).Vector();
	//FVector DiffVector = PlayerPawn->GetActorLocation() - Cast<ACharacter>(ControlledPawn)->GetMesh()->GetSocketLocation(TEXT("head"));

	FVector HeadVector = ControlledPawn->GetActorRotation().Vector();
	FVector DiffVector = PlayerPawn->GetActorLocation() - ControlledPawn->GetActorLocation();

	float angle = UKismetMathLibrary::Acos(FVector::DotProduct(HeadVector, DiffVector)/HeadVector.Size()/DiffVector.Size());

	angle *= (180./(UKismetMathLibrary::Atan(1.)*4.));

	// UE_LOG(LogTemp, Warning, TEXT("%f"), angle);

    if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn) && angle < 60. && Distance < 1000.) {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
    } else {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }
}

