// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "buildings/BuildingBase.h"
#include "Turret.generated.h"

class ACharacterBase;
class AProjectileBase;
class UNiagaraSystem;

UCLASS()
class ALPHAONE_API ATurret : public ABuildingBase
{
    GENERATED_BODY()

public:
    ATurret();
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    void RotateTurret(FVector LookAtTarget);
    void Fire();

private:
    void CheckFireCondition();
    float GetDistanceToEnemy();

    ACharacterBase* EnemyTarget = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent*      ProjectileSpawnPoint = nullptr;

    //projectile of this pawn
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AProjectileBase> ProjectileClass;

    //particle system effect when the pawn is destroyed
    UPROPERTY(EditAnywhere, Category = "Effects")
    UNiagaraSystem* MuzzleParticle = nullptr;

    FTimerHandle FireRateTimerHandle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    float FireRate = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    float FireRange = 1000.0f;
};




