// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "characters/CharacterBase.h"
#include "Archer.generated.h"

class AProjectileBase;
class UAnimMontage;
class UAnimInstance;

UCLASS()
class ALPHAONE_API AArcher : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AArcher();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Fire();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void AttackStatusOff();

	//projectile of this pawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;

	//the location where the projectile will spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* NormalAttackMontage;

	UPROPERTY(EditAnywhere)
	float NormalAttackRate = 1.0f;

	//used to play animation montage
	UAnimInstance* AnimInstance;

	FTimerHandle FireRateTimerHandle;
};


