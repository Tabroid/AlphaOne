// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "GameFramework/Actor.h"
#include "characters/CharacterBase.h"
#include "CollisionQueryParams.h"
#include "WeaponBase.generated.h"


USTRUCT(BlueprintType)
struct FWeaponSockets
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BindingSockets")
    FName Name = "";
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BindingSockets")
	float Radius = 0;

	FVector Location_Prev;
	FRotator Rotation_Prev;
	FTraceHandle QueryHandle_Prev;
};

USTRUCT(BlueprintType)
struct FWeaponMontages
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    TArray<FWeaponSockets> Sockets;
};


UCLASS()
class ALPHAONE_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();
	virtual ~AWeaponBase();

	virtual void Tick(float DeltaTime) override;
	virtual ACharacterBase* GetCharacter() const { return MyCharacter; }
	virtual void AttachToCharacter(ACharacterBase* Character);
	virtual void DetachFromCharacter();

	virtual bool Attack();
	virtual void AttackEnd(bool ResetCombo = false) { if (ResetCombo) { AttackCombo = 0; } else { AttackCombo++; } }

	virtual TSubclassOf<UDamageType> GetDamageType() const { return DamageType; }

	virtual void OnSweepBegin();
	virtual void OnSweepComplete(const FTraceHandle &Handle, FTraceDatum &Data);
	virtual void OnSweepEnd();
	virtual void RequestAsyncSweep(int32 Index);

	// a pure virtual function to be overrided
	virtual bool ShootProjectile() { return false; }

	// @TODO: implment
	float GetDamage() const { return 50; }

	UFUNCTION(BlueprintCallable)
	FVector GetSocketLocation(FName name) const;

	UFUNCTION(BlueprintCallable)
	FRotator GetSocketRotation(FName name) const;

	UFUNCTION(BlueprintCallable)
	const TArray<FWeaponSockets> &GetCollisionSockets() const;
	TArray<FWeaponSockets>& AWeaponBase::GetCollisionSockets();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	ACharacterBase* MyCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<FWeaponMontages> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	int32 AttackCombo;

	UPROPERTY(EditAnywhere)
	float AttackRate = 1.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> DamageType;

	// Used for assigning the collision mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CollisionMesh;

	FCollisionQueryParams QueryParams;
	TArray<AActor*> HitActors;

	uint8 bWantsToSweep : 1;
	FTraceDelegate SweepDelegate;

private:
	void SweepDebugDrawings(const FTraceDatum &TraceData, bool Persist = false, float LifeTime = 10.0f, uint8 Depth = 0, float Thickness = 1.0f);
};
