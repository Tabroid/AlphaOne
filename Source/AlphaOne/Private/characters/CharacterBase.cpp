// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/CharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
: CharacterLevel(1)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttributeSet = CreateDefaultSubobject<UCharacterAttributes>(TEXT("AttributeSet"));
	bInitialized = true;
	SetType(EUnitTypes::Mob);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AttributeSet->NaturalChange(DeltaTime);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacterBase::TurnRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacterBase::LookUpRate);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACharacterBase::OnStartAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ACharacterBase::OnStopAttack);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACharacterBase::OnStartSprinting);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACharacterBase::OnStopSprinting);
	PlayerInputComponent->BindAction("RunToggle", IE_Pressed, this, &ACharacterBase::OnStartSprintingToggle);

}


void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ACharacterBase::UnPossessed()
{
}

void ACharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, CharacterLevel);
}

float ACharacterBase::GetHealth() const
{
	if (!AttributeSet)
	return 1.f;

	return AttributeSet->GetHealth();
}

float ACharacterBase::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float ACharacterBase::GetMana() const
{
	return AttributeSet->GetMana();
}

float ACharacterBase::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float ACharacterBase::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

int32 ACharacterBase::GetCharacterLevel() const
{
	return CharacterLevel;
}

bool ACharacterBase::SetCharacterLevel(int32 NewLevel)
{
	if (CharacterLevel != NewLevel && NewLevel > 0) {
		CharacterLevel = NewLevel;
		// do changes for level change
		return true;
	}
	return false;
}

void ACharacterBase::SetAction(EUnitActions NewAction, bool State)
{
	if (State) {
		AttributeSet->Action |= NewAction;
	} else {
		AttributeSet->Action &= ~NewAction;
	}
}

void ACharacterBase::SetStatus(EUnitStatuses NewStatus, bool State)
{
	if (State) {
		AttributeSet->Status |= NewStatus;
	} else {
		AttributeSet->Status &= ~NewStatus;
	}
}

void ACharacterBase::SetType(EUnitTypes NewType)
{
	AttributeSet->Type = NewType;
}

FGenericTeamId ACharacterBase::GetGenericTeamId() const
{
	static const FGenericTeamId PlayerTeam(0);
	static const FGenericTeamId AITeam(1);
	return Cast<APlayerController>(GetController()) ? PlayerTeam : AITeam;
}

void ACharacterBase::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ACharacterBase::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void ACharacterBase::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::TurnRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::OnStartAttack()
{
	bWantsToAttack = true;
	Attack();
	SetSprinting(false, false);
}

void ACharacterBase::OnStopAttack()
{
	bWantsToAttack = false;
}

void ACharacterBase::SetSprinting(bool bNewSprinting, bool bToggle)
{
	bWantsToSprint = bNewSprinting;
	bWantsToSprintToggled = bNewSprinting && bToggle;
}

void ACharacterBase::OnStartSprinting()
{
	OnStopAttack();
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	SetSprinting(true, false);
}

void ACharacterBase::OnStartSprintingToggle()
{
	OnStopAttack();
	SetSprinting(true, true);
}

void ACharacterBase::OnStopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	SetSprinting(false, false);
}

bool ACharacterBase::Attack()
{
	if (!IsAbleToAct()) {
		return false;
	}
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	auto AnimInstance = GetMesh()->GetAnimInstance();
	if (PlayerController && !(GetAction() & EUnitActions::Attacking) && AnimInstance && NormalAttackMontage) {
		SetAction(EUnitActions::Attacking, true);
		AnimInstance->Montage_Play(NormalAttackMontage, NormalAttackRate);
		FOnMontageEnded MontageEndDelegate;
    	MontageEndDelegate.BindUObject(this, &ACharacterBase::OnPlayAttackEnd);
		AnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
		return true;
	}
	return false;
}

void ACharacterBase::OnPlayAttackEnd(UAnimMontage* montage, bool interrupted)
{
	SetAction(EUnitActions::Attacking, false);
	if (!interrupted && bWantsToAttack) {
		Attack();
	}
}

float ACharacterBase::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsAbleToAct()) {
		return 0.f;
	}
	auto hp = AttributeSet->GetHealth() - DamageAmount;
	AttributeSet->SetHealth(hp);
	if (hp <= 0.) {
		Die(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	return DamageAmount;
}

bool ACharacterBase::IsAbleToAct() const
{
	if (GetAction() == EUnitActions::Dying				// already dying
		|| IsPendingKill()								// object already destroyed
		|| GetWorld()->GetAuthGameMode<AAlphaOneGameModeBase>() == NULL) {
		return false;
	}

	return true;
}

bool ACharacterBase::Die(float KillingDamage, const FDamageEvent& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	if (!IsAbleToAct()) {
		return false;
	}

	GetCharacterMovement()->ForceReplicationUpdate();

	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	Killer = GetDamageInstigator(Killer, *DamageType);
	APawn *KillerPawn = Killer ? Killer->GetPawn() : nullptr;

	OnDeath(KillingDamage, DamageEvent, KillerPawn, DamageCauser);
	return true;
}

void ACharacterBase::OnDeath(float KillingDamage, const FDamageEvent& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	SetAction(EUnitActions::Dying);
	SetReplicatingMovement(false);
	TearOff();

	// @TODO play sound
	// @TODO death penalty
	// @TODO change view

	DetachFromControllerPendingDestroy();
	StopAllAnimMontages();

	if (GetMesh()) {
		static FName CollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetCollisionProfileName(CollisionProfileName);
	}
	SetActorEnableCollision(true);

	// Death anim
	auto AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage) {
		float DeathAnimDuration = AnimInstance->Montage_Play(DeathMontage);
		// Ragdoll
		if (DeathAnimDuration > 0.f) {
			// Trigger ragdoll a little before the animation early so the character doesn't
			// blend back to its normal position.
			const float TriggerRagdollTime = DeathAnimDuration - 0.7f;

			// Enable blend physics so the bones are properly blending against the montage.
			GetMesh()->bBlendPhysics = true;

			// Use a local timer handle as we don't need to store it for later but we don't need to look for something to clear
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ACharacterBase::SetRagdollPhysics, FMath::Max(0.1f, TriggerRagdollTime), false);
		} else {
			SetRagdollPhysics();
		}
	}

	// disable collisions on capsule
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	// @TODO need test the right timing to call destroy
	// Destroyed();
}

void ACharacterBase::SetRagdollPhysics()
{
	bool bInRagdoll = false;

	if (IsPendingKill()) {
		bInRagdoll = false;
	} else if (!GetMesh() || !GetMesh()->GetPhysicsAsset()) {
		bInRagdoll = false;
	} else {
		// initialize physics/etc
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;
		bInRagdoll = true;
	}

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	if (!bInRagdoll) {
		// hide and set short lifespan
		TurnOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(1.0f);
	} else {
		SetLifeSpan(10.0f);
	}
}


void ACharacterBase::StopAllAnimMontages()
{
	auto AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Stop(0.0f);
}