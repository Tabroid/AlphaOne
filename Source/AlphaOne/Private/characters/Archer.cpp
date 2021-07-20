// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/Archer.h"
#include "Projectile/Arrow.h"

// Sets default values
AArcher::AArcher()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->AttachTo(GetMesh(), TEXT("bow_base"));
}

// Called when the game starts or when spawned
void AArcher::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArcher::Fire() 
{
	if (ProjectileClass){
		//spawning the projectile at the spawn point
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		AArrow* TempProjectile = 
		GetWorld()->SpawnActor<AArrow>(ProjectileClass, SpawnLocation, SpawnRotation);
		//set the owner 
		TempProjectile->SetOwner(this);
	}
}

// Called every frame
void AArcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AArcher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AArcher::Fire);
}