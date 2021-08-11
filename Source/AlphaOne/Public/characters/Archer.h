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
	virtual USceneComponent* GetProjectileSpawnComponent() const { return ProjectileSpawnPoint; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void CameraIn();
	void CameraOut();

	//the location where the projectile will spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FName ProjectileSpawnSocket;
};




