// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "GameFramework/Actor.h"
#include "GameplayTags.h"
#include "Components/ArrowComponent.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ALPHAONE_API AProjectileBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AProjectileBase();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    virtual float GetCollisionSize() const;

    // Returns current mana
    UFUNCTION(BlueprintCallable)
    virtual float GetMoveSpeed() const;

    // Returns maximum mana
    UFUNCTION(BlueprintCallable)
    virtual float GetMass() const;

    UFUNCTION(BlueprintCallable)
    virtual void AddIgnoreActors(TArray<AActor*> IgnoreActors);

    UFUNCTION(BlueprintCallable)
    virtual void ClearIgnoreActors();

    UFUNCTION(BlueprintCallable)
    FORCEINLINE FVector GetSpawnLocation() const { return SpawnPoint->GetRelativeLocation(); }

    UFUNCTION(BlueprintCallable)
    FORCEINLINE FRotator GetSpawnRotation() const { return SpawnPoint->GetRelativeRotation(); }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void HandleCollesionSizeChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
    virtual void HandleMassChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
    virtual void HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

    // called when movement speed changes
    UFUNCTION(BlueprintImplementableEvent)
    void OnCollesionSizeChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

    UFUNCTION(BlueprintImplementableEvent)
    void OnMassChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

    UFUNCTION(BlueprintImplementableEvent)
    void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

    UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
    USphereComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* ProjectileMovement;

    //used for assigning the projectile mesh
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    UArrowComponent* SpawnPoint;

    //particle effect when the projectile hit
    UPROPERTY(EditAnywhere, Category = "Effects")
    UNiagaraComponent* ParticleTrail = nullptr;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UNiagaraSystem* HitParticle = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    float CollisionSize = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
    float MoveSpeed = 5000.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
    float Mass = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float Damage = 50.f;

    UPROPERTY()
    bool bInitialized = false;

    //used for the damage of the projectile
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    TSubclassOf<UDamageType> DamageType;

private:
    AController* GetOwnerController() const;

    UFUNCTION()
    virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};