// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "abilities/AlphaOneAbilitySystem.h"
#include "characters/CharacterAttributes.h"
#include "weapons/WeaponBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Templates/SharedPointer.h"
#include "CharacterBase.generated.h"

UCLASS()
class ALPHAONE_API ACharacterBase : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable)
	virtual AWeaponBase *GetCurrentWeapon() const;

	UFUNCTION(BlueprintCallable)
	virtual bool EquipWeapon(AWeaponBase* NewWeapon = nullptr);

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
	virtual float GetMoveSpeed() const;

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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// @TODO: item slots, equipment slots
	// setupItems()
	UFUNCTION(BlueprintCallable)
	virtual bool IsAbleToAct() const;
	virtual bool Attack();
	void SetSprinting(bool bNewSprinting, bool bToggle);
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool Die(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual void StopAllAnimMontages();
	// Controller responses
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void TurnRate(float AxisValue);

	UFUNCTION(BlueprintCallable)
	virtual void OnAttackEnd(bool Interrupted = false);

	UFUNCTION(BlueprintCallable)
	UFactionComponent* GetFactionComponent() { return FactionSystemComponent; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags) { /* place holder */ }
	virtual void OnManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags) { /* place holder */ }
	virtual void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags) { /* place holder */ }
	virtual void OnStartAttack();
	virtual void OnStopAttack();
	virtual void OnStartSprinting();
	virtual void OnStartSprintingToggle();
	virtual void OnStopSprinting();
	virtual void OnDeath(float KillingDamage, const FDamageEvent& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);
	virtual void SetRagdollPhysics();

	// properties of the character
	UPROPERTY()
	UCharacterAttributes* AttributeSet;

	UPROPERTY(Transient)
	uint8 bInitialized : 1;

	// current running state
	UPROPERTY(Transient, BlueprintReadOnly, Replicated)
	uint8 bWantsToSprint : 1;

	UPROPERTY(Transient, BlueprintReadOnly)
	uint8 bWantsToSprintToggled : 1;

	UPROPERTY(Transient, BlueprintReadOnly)
	uint8 bWantsToAttack : 1;

	UPROPERTY()
	UAlphaOneAbilitySystem* AbilitySystemComponent;

	// Passive gameplay effects applied on creation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	virtual FGenericTeamId GetGenericTeamId() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeaponBase> DefaultWeapon;

	AWeaponBase* DefaultWeaponPtr;

	AWeaponBase* CurrentWeapon;

	// A proxy to FactionSystem component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction", meta = (AllowPrivateAccess = "true"))
	EUnitFactions DefaultFaction;

	UPROPERTY()
	UFactionComponent* FactionSystemComponent;

private:
	UPROPERTY(EditAnywhere)
	float RotationRate = 10.0f;

	UPROPERTY(EditAnywhere)
	float JogSpeed = 350.0f;

	UPROPERTY(EditAnywhere)
	float SprintSpeed = 700.0f;
};
