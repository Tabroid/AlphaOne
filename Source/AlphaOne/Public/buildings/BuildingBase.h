// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "abilities/AlphaOneAbilitySystem.h"
#include "characters/CharacterAttributes.h"
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
	virtual float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMana() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxMana() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetManaPercentage() const;

	UFUNCTION(BlueprintCallable)
	virtual int32 GetLevel() const;

	UFUNCTION(BlueprintCallable)
	virtual bool SetLevel(int32 NewLevel);

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UCharacterAttributes* AttributeSet;

	UPROPERTY()
	UAlphaOneAbilitySystem* AbilitySystemComponent;

	void RotateHealthBar();

	//various component for assigning mesh later on
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent*    CapsuleComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AuraMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBar = nullptr;

private:
	ACharacterBase* PlayerCharacter;
};
