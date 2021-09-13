// Fill out your copyright notice in the Description page of Project Settings.


#include "weapons/KnockbackArrow.h"

AKnockbackArrow::AKnockbackArrow()
    : AProjectileBase ()
{
    PrimaryActorTick.bCanEverTick = false;

    ShockWaveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shock Wave"));
    //RootComponent = ProjectileMesh;
    ShockWaveMesh->SetupAttachment(RootComponent);
}

void AKnockbackArrow::OnImpact(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& Hit)
{
}


