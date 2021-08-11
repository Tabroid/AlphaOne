// Fill out your copyright notice in the Description page of Project Settings.

#include "weapons/ProjectileBase.h"
// UE4
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"


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
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	RootComponent = CollisionComp;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProjectileMesh->SetCollisionObjectType(COLLISION_PROJECTILE);
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = MoveSpeed;
	ProjectileMovement->MaxSpeed     = MoveSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = Mass;
	bInitialized = true;


	ParticleTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetCollisionResponseToAllChannels(ECR_Ignore);
	ParticleTrail->SetCollisionObjectType(COLLISION_PROJECTILE);
	ParticleTrail->SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Point"));
	SpawnPoint->SetupAttachment(RootComponent);
	//if (ParticleTrail)
	//UNiagaraFunctionLibrary::SpawnSystemAttached(ParticleTrail, RootComponent,
	//TEXT("SphereComp"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepRelativeOffset, true);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	//dynamic binding the OnHit function when the projectile hit something
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	SetLifeSpan(20.f);
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
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AActor* MyOwner = GetOwner();
	if (IsValid(MyOwner) && IsValid(OtherActor)) {
		//apply the damage to the target that the projectile hits.
		//UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
		auto FactionComp = MyOwner->FindComponentByClass<UFactionComponent>();
		if (FactionComp && FactionComp->IsEnemy(OtherActor)) {
			FPointDamageEvent DamageEvent(Damage, Hit, NormalImpulse, DamageType);
			OtherActor->TakeDamage(Damage, DamageEvent, GetOwnerController(), MyOwner);
			if (HitParticle) {
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitParticle, CollisionComp->GetComponentLocation());
			}
		}
		SetLifeSpan(2.f);
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
	ProjectileMovement->ProjectileGravityScale = GetMass();
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

void AProjectileBase::AddIgnoreActors(TArray<AActor*> IgnoreActors)
{
	for (auto Actor : IgnoreActors) {
		CollisionComp->IgnoreActorWhenMoving(Actor, true);
	}
}

void AProjectileBase::ClearIgnoreActors()
{
	CollisionComp->ClearMoveIgnoreActors();
}