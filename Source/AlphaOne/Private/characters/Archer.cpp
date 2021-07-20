// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/Archer.h"
#include "weapons/ProjectileBase.h"

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
	
	//get the animation instance from the skeletal mesh
	AnimInstance = GetMesh()->GetAnimInstance();

	bIsAttacking = false;
}

void AArcher::Fire() 
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	//bWantToAttack = PlayerController->IsInputKeyDown(EKeys::LeftMouseButton);
	if (PlayerController){
		//while(PlayerController->IsInputKeyDown(EKeys::LeftMouseButton)){
			if (!bIsAttacking && ProjectileClass && AnimInstance && NormalAttackMontage){
				//spawning the projectile at the spawn point
				FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
				FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
				AProjectileBase* TempProjectile = 
				GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
				//set the owner 
				TempProjectile->SetOwner(this);
				bIsAttacking = true;
				float PlayTime = AnimInstance->Montage_Play(NormalAttackMontage, NormalAttackRate);
				GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AArcher::AttackStatusOff, PlayTime/NormalAttackRate, true);
			}
		//}
	}
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

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AArcher::Fire);
}
