// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "abilities/AlphaOneAbilitySystem.h"
#include "GameFramework/Actor.h"
#include "BuildingBase.generated.h"

class UCapsuleComponent;
class UWidgetComponent;
class ACharacterBase;

UCLASS()
class ALPHAONE_API ABuildingBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildingBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable)
	UCharacterAttributes* GetAttributes() { return AttributeSet; }

	UFUNCTION(BlueprintCallable)
	EUnitActions GetAction() const { return AttributeSet->Action; }

	UFUNCTION(BlueprintCallable)
	void SetAction(EUnitActions NewAction, bool State = true);

	UFUNCTION(BlueprintCallable)
	EUnitStatuses GetStatus() const { return AttributeSet->Status; }

	UFUNCTION(BlueprintCallable)
	void SetStatus(EUnitStatuses NewStatus, bool State = true);

	UFUNCTION(BlueprintCallable)
	EUnitTypes GetType() const { return AttributeSet->Type; }

	UFUNCTION(BlueprintCallable)
	void SetType(EUnitTypes NewType);

	UFUNCTION(BlueprintCallable)
	UFactionComponent* GetFactionComponent() { return FactionSystemComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RotateHealthBar();

	UPROPERTY()
	UCharacterAttributes* AttributeSet;

	UPROPERTY()
	UAlphaOneAbilitySystem* AbilitySystemComponent;

	//various component for assigning mesh later on
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent*    CapsuleComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AuraMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBar = nullptr;

	// A proxy to FactionSystem component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction", meta = (AllowPrivateAccess = "true"))
	EUnitFactions DefaultFaction;

	UPROPERTY()
	UFactionComponent* FactionSystemComponent;

private:
	ACharacterBase* PlayerCharacter;
};
