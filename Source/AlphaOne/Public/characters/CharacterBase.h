// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "GameplayEffect.h"
#include "abilities/AlphaOneAbilitySystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Templates/SharedPointer.h"
#include "CharacterBase.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EControllStates: uint8
{
    NONE            = 0          UMETA(Hidden),
    WantsToAttack   = 1 << 0     UMETA(DisplayName = "Wants to attack"),
    WantsToSprint   = 1 << 1     UMETA(DisplayName = "Wants to sprint"),
};
ENUM_CLASS_FLAGS(EControllStates);

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
	virtual bool EquipWeapon(AWeaponBase* NewWeapon = nullptr);

	UFUNCTION(BlueprintCallable)
	void SetAction(EUnitActions Action, bool NewState = true);

	UFUNCTION(BlueprintCallable)
	void SetStatus(EUnitStatuses Status, bool NewState = true);

	UFUNCTION(BlueprintCallable)
	void SetControll(EControllStates Controll, bool NewState = true);

	UFUNCTION(BlueprintCallable)
	void SetType(EUnitTypes NewType);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual AWeaponBase *GetCurrentWeapon() const;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCharacterAttributes* GetAttributes() { return AttributeSet; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckType(EUnitTypes Type) const { return (AttributeSet->Type == Type); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckAction(EUnitActions Action) const { return static_cast<bool>(AttributeSet->Action & Action); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckStatus(EUnitStatuses Status) const { return static_cast<bool>(AttributeSet->Status & Status); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckControll(EControllStates Controll) const { return static_cast<bool>(ControllState & Controll); }


	// @TODO: item slots, equipment slots
	// setupItems()
	UFUNCTION(BlueprintCallable)
	virtual bool IsAbleToAct() const;
	virtual bool Attack();
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool Die(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual void StopAllAnimMontages();
	// Controller responses
	void Jump() override;
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void TurnRate(float AxisValue);

	UFUNCTION(BlueprintCallable)
	virtual void OnAttackEnd(bool Interrupted = false);

	UFUNCTION(BlueprintCallable)
	UFactionComponent* GetFactionComponent() { return FactionSystemComponent; }

	UFUNCTION(BlueprintCallable)
	void SetJogSpeed(float Value);
	UFUNCTION(BlueprintCallable)
	float GetJogSpeed() const { return JogSpeed; }

	UFUNCTION(BlueprintCallable)
	void SetSprintSpeed(float Value) { SprintSpeed = Value; }
	UFUNCTION(BlueprintCallable)
	float GetSprintSpeed() const { return SprintSpeed; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags) { /* place holder */ }
	virtual void OnManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags) { /* place holder */ }
	virtual void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags) { /* place holder */ }
	virtual void OnStartAttack();
	virtual void OnStopAttack();
	virtual void OnStartSprinting();
	virtual void OnStopSprinting();
	virtual void OnDeath(float KillingDamage, const FDamageEvent& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);
	virtual void SetRagdollPhysics();

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

	// properties of the character
	UPROPERTY()
	UCharacterAttributes* AttributeSet;
	UPROPERTY()
	UAlphaOneAbilitySystem* AbilitySystemComponent;

	UPROPERTY()
	EControllStates ControllState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float DefaultArmLength = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FName UnitDataRowName = "Default_Character";

private:
	float JogSpeed = 350.f;
	float SprintSpeed = 700.f;
};
