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

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform ,TEXT("bow_base"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AArcher::BeginPlay()
{
	Super::BeginPlay();

	bIsAttacking = false;
}

void AArcher::AttackStatusOff() 
{
	bIsAttacking = false;
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

bool AArcher::Attack()
{
	if (Projectile && Super::Attack()) {
		//spawning the projectile at the spawn point
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		AProjectileBase* TempProjectile = 
		GetWorld()->SpawnActor<AProjectileBase>(Projectile, SpawnLocation, SpawnRotation);
		//set the owner 
		TempProjectile->SetOwner(this);
		return true;
	}
	return false;
}

void AArcher::CameraIn() 
{
	SpringArm->TargetArmLength = 0.0f;
}

void AArcher::CameraOut() 
{
	SpringArm->TargetArmLength = 200.0f;
}