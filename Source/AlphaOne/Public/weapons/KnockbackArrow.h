// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "weapons/ProjectileBase.h"
#include "KnockbackArrow.generated.h"


UCLASS()
class ALPHAONE_API AKnockbackArrow : public AProjectileBase
{
    GENERATED_BODY()
public:
    // Sets default values for this actor's properties
    AKnockbackArrow();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* ShockWaveMesh;

private:
    UFUNCTION()
    virtual void OnImpact(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};



