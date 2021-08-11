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

	// @TODO move this to weapon if weapon mesh implemented
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform ,TEXT("bow_base"));

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
	PlayerInputComponent->BindAction("Targeting", EInputEvent::IE_Pressed, this, &AArcher::CameraIn);
	PlayerInputComponent->BindAction("Targeting", EInputEvent::IE_Released, this, &AArcher::CameraOut);
}

void AArcher::CameraIn()
{
	CameraBoom->TargetArmLength = 100.0f;
}

void AArcher::CameraOut()
{
	CameraBoom->TargetArmLength = 300.0f;
}