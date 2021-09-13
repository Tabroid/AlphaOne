// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/Archer.h"
#include "weapons/ProjectileBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AArcher::AArcher()
{
     // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SetType(EUnitTypes::Hero);
}

// Called when the game starts or when spawned
void AArcher::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AArcher::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// aiming
void AArcher::OnStartSpecial()
{
    SetAction(EUnitActions::Aiming);
    GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed() * CombatMoveSpeedFactor;
    CameraArm->TargetArmLength = DefaultArmLength; // 100.f;
    // In aiming mode, actor rotates with view direction
    bUseControllerRotationYaw = false;
}

// aiming off
void AArcher::OnStopSpecial()
{
    SetAction(EUnitActions::Aiming, false);
    GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
    CameraArm->TargetArmLength = DefaultArmLength;
    bUseControllerRotationYaw = false;
}
