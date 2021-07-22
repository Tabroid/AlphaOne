// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "characters/CharacterBase.h"
#include "Archer.generated.h"

class AProjectileBase;
class UAnimMontage;
class USpringArmComponent;
class UCameraComponent;

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

	virtual bool Attack() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void CameraIn();
	void CameraOut();

	//projectile of this pawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> Projectile;

	//the location where the projectile will spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm = nullptr;
	//camera that the player look through
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera = nullptr;

	FTimerHandle FireRateTimerHandle;
};




