// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "abilities/AbilityBase.h"
#include "characters/CharacterAttributes.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GenericTeamAgentInterface.h"
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

	// Returns current health, will be 0 if dead
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;

	// Returns maximum health
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	// Returns current mana
	UFUNCTION(BlueprintCallable)
	virtual float GetMana() const;

	// Returns maximum mana
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxMana() const;

	// Returns current movement speed
	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const;

	// Returns the character level that is passed to the ability system
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	// Modifies the character level, this may change abilities. Returns true on success
	UFUNCTION(BlueprintCallable)
	virtual bool SetCharacterLevel(int32 NewLevel);

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

	virtual void OnPlayAttackEnd(UAnimMontage* montage, bool interrupted);


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

	// The level of this character, should not be modified directly once it has already spawned
	UPROPERTY(EditAnywhere, Replicated, Category = Abilities)
	int32 CharacterLevel;

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

	// Passive gameplay effects applied on creation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	// TODO: gamplay abilities
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	// TArray<TSubclassOf<UAbilityBase>> GameplayAbilities;

	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	UPROPERTY(EditAnywhere)
	float RotationRate = 10.0f;

	UPROPERTY(EditAnywhere)
	float NormalAttackRate = 1.0f;

	UPROPERTY(EditAnywhere)
	float JogSpeed = 350.0f;

	UPROPERTY(EditAnywhere)
	float SprintSpeed = 700.0f;

	// @TODO: use a character animation manager to manage all montages
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* NormalAttackMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage = nullptr;
};




