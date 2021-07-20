// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Projectile/Arrow.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->AlwaysLoadOnClient = true;
	CollisionComp->AlwaysLoadOnServer = true;
	CollisionComp->bTraceComplexOnMove = true;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	RootComponent = CollisionComp;

	//dynamic binding the OnHit function when the projectile hit something
	CollisionComp->OnComponentHit.AddDynamic(this, &AArrow::OnHit);


	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	//RootComponent = ProjectileMesh;
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed     = MovementSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;

}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

AController* AArrow::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}

void AArrow::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
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
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

