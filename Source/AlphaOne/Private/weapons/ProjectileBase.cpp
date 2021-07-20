// Fill out your copyright notice in the Description page of Project Settings.

#include "weapons/ProjectileBase.h"
// UE4
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
 	// no need to tick
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(CollisionSize);
	CollisionComp->AlwaysLoadOnClient = true;
	CollisionComp->AlwaysLoadOnServer = true;
	CollisionComp->bTraceComplexOnMove = true;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	RootComponent = CollisionComp;

	//dynamic binding the OnHit function when the projectile hit something
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);


	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	//RootComponent = ProjectileMesh;
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = MoveSpeed;
	ProjectileMovement->MaxSpeed     = MoveSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	bInitialized = true;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

AController* AProjectileBase::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}

float AProjectileBase::GetCollisionSize() const
{
	return CollisionSize;
}

float AProjectileBase::GetMoveSpeed() const
{
	return MoveSpeed;
}

float AProjectileBase::GetMass() const
{
	return Mass;
}


void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
			   FVector NormalImpulse, const FHitResult& Hit) 
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;
	if (OtherActor && OtherActor != this && OtherActor != MyOwner){
		//apply the damage to the target that the projectile hits.
		//UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
		FPointDamageEvent DamageEvent(Damage, Hit, NormalImpulse, DamageType);
		OtherActor->TakeDamage(Damage, DamageEvent, GetOwnerController(), GetOwner());
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());

	    //Destroy();
	}
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::HandleCollesionSizeChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	// Update the collision size
	CollisionComp->InitSphereRadius(GetCollisionSize());
	if (bInitialized) {
		OnCollesionSizeChanged(DeltaValue, EventTags);
	}
}

void AProjectileBase::HandleMassChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	// @TODO mass is for physics
	if (bInitialized) {
		OnMassChanged(DeltaValue, EventTags);
	}
}

void AProjectileBase::HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	// Update the movement speed
	ProjectileMovement->MaxSpeed = GetMoveSpeed();
	if (bInitialized) {
		OnMoveSpeedChanged(DeltaValue, EventTags);
	}
}

