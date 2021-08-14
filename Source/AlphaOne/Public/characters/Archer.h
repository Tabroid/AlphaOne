// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "characters/AlphaOnePlayer.h"
#include "Archer.generated.h"

class AProjectileBase;
class UAnimMontage;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ALPHAONE_API AArcher : public AAlphaOnePlayer
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AArcher();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // virtual void OnCameraResetEnd_Impl() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void AimingOn();
	virtual void AimingOff();
};




