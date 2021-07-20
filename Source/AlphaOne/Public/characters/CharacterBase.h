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

	UCharacterAttributes* GetAttributes() { return AttributeSet; }

	// @TODO: item slots, equipment slots

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// called when character takes damage
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
	    	       ACharacterBase* InstigatorCharacter, AActor* DamageCauser);

	// called when health changes
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// called when mana changes
	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// called when movement speed changes
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// handle functions that can be overrided by inherited classes
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
							  ACharacterBase* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// The level of this character, should not be modified directly once it has already spawned
	UPROPERTY(EditAnywhere, Replicated, Category = Abilities)
	int32 CharacterLevel;

	// properties of the character
	UPROPERTY()
	UCharacterAttributes* AttributeSet;

	UPROPERTY()
	bool bInitialized;

	// Passive gameplay effects applied on creation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	// TODO: gamplay abilities
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	// TArray<TSubclassOf<UAbilityBase>> GameplayAbilities;

	virtual FGenericTeamId GetGenericTeamId() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
