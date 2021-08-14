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

void AArcher::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Targeting", EInputEvent::IE_Pressed, this, &AArcher::AimingOn);
	PlayerInputComponent->BindAction("Targeting", EInputEvent::IE_Released, this, &AArcher::AimingOff);
}

void AArcher::AimingOn()
{

	SetAction(EUnitActions::Aiming);
	OnStopSprinting();
	CameraArm->TargetArmLength = DefaultArmLength; // 100.f;
	// In aiming mode, actor rotates with view direction
	bUseControllerRotationYaw = true;
}

void AArcher::AimingOff()
{
	CameraArm->TargetArmLength = DefaultArmLength;
	SetAction(EUnitActions::Aiming, false);
	bUseControllerRotationYaw = false;
}

void AArcher::OnStartSprinting()
{
	AimingOff();
	Super::OnStartSprinting();
}
