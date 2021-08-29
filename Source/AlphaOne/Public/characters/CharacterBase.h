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

class UAnimInstance;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EControlStates: uint8
{
    NONE            = 0          UMETA(Hidden),
    WantsToAttack   = 1 << 0     UMETA(DisplayName = "Wants to attack"),
    WantsToSprint   = 1 << 1     UMETA(DisplayName = "Wants to sprint"),
	WantsToAim		= 1 << 2	 UMETA(DisplayName = "Wants to aim"),
	WantsToClimb    = 1 << 3	 UMETA(DisplayName = "Wants to climb"),
	WantsToSwim     = 1 << 4	 UMETA(DisplayName = "Wants to swim"),
	WantsToJump     = 1 << 5	 UMETA(DisplayName = "Wants to jump"),
	WantsToTurn		= 1 << 6	 UMETA(DisplayName = "Wants to turn"),
};
ENUM_CLASS_FLAGS(EControlStates);

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
	void SetControl(EControlStates Control, bool NewState = true);

	UFUNCTION(BlueprintCallable)
	void SetType(EUnitTypes NewType);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual AWeaponBase *GetCurrentWeapon() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCharacterAttributes* GetAttributes() { return AttributeSet; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckType(EUnitTypes Type) const { return (AttributeSet->Type == Type); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckAction(EUnitActions Action) const { return static_cast<bool>(AttributeSet->Action & Action); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckStatus(EUnitStatuses Status) const { return static_cast<bool>(AttributeSet->Status & Status); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckControl(EControlStates Control) const { return static_cast<bool>(ControlState & Control); }

	// @TODO: item slots, equipment slots
	// setupItems()
	UFUNCTION(BlueprintCallable)
	virtual bool IsAbleToAct() const;
	virtual bool Attack();
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
	// virtual void OnStartSprinting();
	// virtual void OnStopSprinting();
	virtual void OnDeath(float KillingDamage, const FDamageEvent& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);
	virtual void SetRagdollPhysics();

	// Passive gameplay effects applied on creation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	virtual FGenericTeamId GetGenericTeamId() const override;

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
	EControlStates ControlState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FName UnitDataRowName = "Default_Character";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage = nullptr;

private:
	UAnimInstance *AnimInstance;

// variables and functions for animations
public:
	UFUNCTION(BlueprintCallable)
	void SetMoveSpeed(float Value);
	UFUNCTION(BlueprintCallable)
	float GetMoveSpeed() const { return MoveSpeed; }

	UFUNCTION(BlueprintCallable)
	void SetCombatMoveSpeedFactr(float Value) { CombatMoveSpeedFactor = Value; }
	UFUNCTION(BlueprintCallable)
	float GetCombatMoveSpeedFactr() const { return CombatMoveSpeedFactor; }

protected:

	virtual void AnimationStatesUpdate(float DeltaTime);

	float MoveSpeed = 500.f;
	float CombatMoveSpeedFactor = 1.0f;
	float RotationYawLastTick = 0.f;
	float DistanceCurveValueLastTick = 0.f;
	float DistanceCurveValueInit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FName DistanceCurveName = "DistanceCurve";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float DistanceCurveValueScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float RotationYawOffset = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float VelocitySize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float AccelerationSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	ECardinalDirections MoveDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FRotator AimDeltaRotator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FRotator MoveDeltaRotator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FRotator AccDeltaRotator;
};
