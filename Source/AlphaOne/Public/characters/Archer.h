// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Archer.generated.h"

class AArrow;

UCLASS()
class ALPHAONE_API AArcher : public ACharacter
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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Fire();

private:
	//projectile of this pawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AArrow> ProjectileClass;

	//the location where the projectile will spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent*      ProjectileSpawnPoint = nullptr;


};