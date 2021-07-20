// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;

UCLASS()
class ALPHAONE_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	AController* GetOwnerController() const;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	           FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	//used for assigning the projectile mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	//particle effect when the projectile hit
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* HitParticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float Damage = 50.f;

	//used for the damage of the projectile
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;
};