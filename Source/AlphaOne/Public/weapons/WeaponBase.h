// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "GameFramework/Actor.h"
#include "characters/CharacterBase.h"
#include "WeaponBase.generated.h"

UCLASS()
class ALPHAONE_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();
	virtual ~AWeaponBase();

	virtual ACharacterBase* GetCharacter() const { return MyCharacter; }
	virtual void AttachToCharacter(ACharacterBase* Character);
	virtual void DetachFromCharacter();

	virtual bool Attack();
	virtual void AttackEnd(bool ResetCombo = false) { if (ResetCombo) { AttackCombo = 0; } else { AttackCombo++; } }

	// @TODO: implment
	float GetDamage() const { return 50; }

	UFUNCTION(BlueprintCallable)
	FVector GetSocketLocation(FName name) const;

	UFUNCTION(BlueprintCallable)
	FRotator GetSocketRotation(FName name) const;

	UFUNCTION(BlueprintCallable)
	const TArray<FWeaponSockets> &GetCollisionSockets() const;

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

	// Used for assigning the collision mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CollisionMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
