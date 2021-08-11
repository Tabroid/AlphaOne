// Fill out your copyright notice in the Description page of Project Settings.

#include "buildings/Turret.h"
#include "weapons/ProjectileBase.h"
#include "characters/CharacterBase.h"
#include "Components/WidgetComponent.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ATurret::ATurret()
{
	//create default component for the projectile spawn point
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(AuraMesh);
    BaseMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    BaseMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    // by default it is indestructible
    SetCanBeDamaged(false);
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

    //set up the fire rate timer for the turret
    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ATurret::CheckFireCondition, FireRate, true);
}

void ATurret::CheckFireCondition()
{
    ACharacterBase* NearestEnemy = nullptr;

    TArray<AActor*> AllEnemy;

    //TODO: Need to update to enemy class
    float Distance = 1.e9;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), AllEnemy);
    for (auto Enemy : AllEnemy){
        ACharacterBase* CurrentEnemy = Cast<ACharacterBase>(Enemy);
        if (!CurrentEnemy || CheckStatus(EUnitStatuses::Dead)) continue;
        if (!GetFactionComponent()->IsEnemy(Cast<AActor>(CurrentEnemy))) continue;
        float CurrentDistance = FVector::Dist(CurrentEnemy->GetActorLocation(), GetActorLocation());
        if (CurrentDistance < Distance) {
            Distance = CurrentDistance;
            NearestEnemy = CurrentEnemy;
        }
    }


    if (!NearestEnemy || Distance > FireRange) return;

    RotateTurret(NearestEnemy->GetActorLocation());
    Fire();
}

float ATurret::GetDistanceToEnemy()
{
	if (!EnemyTarget) return 1.e9;
    return FVector::Dist(EnemyTarget->GetActorLocation(), GetActorLocation());
}

void ATurret::Fire()
{
    FTransform SpawnTM(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation());
	auto Projectile = Cast<AProjectileBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTM));
	if (Projectile) {
		Projectile->SetInstigator(this);
		Projectile->SetOwner(this);
        Projectile->AddIgnoreActors({this});
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTM);
	}
}

void ATurret::RotateTurret(FVector LookAtTarget)
{
	if (!AuraMesh) return;
	FVector StartLocation = AuraMesh->GetComponentLocation();
	FRotator AuraRotation = FVector(LookAtTarget - StartLocation).Rotation();
	AuraMesh->SetWorldRotation(AuraRotation);
}
