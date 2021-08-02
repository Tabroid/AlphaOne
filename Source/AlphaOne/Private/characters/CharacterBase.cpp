// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/CharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "weapons/WeaponBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
	: DefaultFaction(EUnitFactions::Protagonist)
{
	PrimaryActorTick.bCanEverTick = true;

	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UAlphaOneAbilitySystem>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	FactionSystemComponent = CreateDefaultSubobject<UFactionComponent>(TEXT("FactionSystemComponent"));

	AttributeSet = CreateDefaultSubobject<UCharacterAttributes>(TEXT("AttributeSet"));
	SetType(EUnitTypes::Mob);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	FactionSystemComponent->SetFaction(DefaultFaction);
	if (DefaultWeapon) {
		DefaultWeaponPtr = GetWorld()->SpawnActor<AWeaponBase>(DefaultWeapon, GetMesh()->GetComponentLocation(), GetMesh()->GetComponentRotation());
		DefaultWeaponPtr->SetOwner(Cast<AActor>(this));
		EquipWeapon(DefaultWeaponPtr);
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AttributeSet->NaturalChange(DeltaTime);
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
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
	if (IsAbleToAct() && CurrentWeapon && !(GetAction() & EUnitActions::Attacking)) {
		return CurrentWeapon->Attack();
	}
	return false;
}

void ACharacterBase::OnAttackEnd(bool Interrupted)
{
	SetAction(EUnitActions::Attacking, false);
	if (CurrentWeapon) {
		CurrentWeapon->AttackEnd(Interrupted);
		if (bWantsToAttack && !Interrupted) {
			Attack();
		}
	}
}

float ACharacterBase::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!CanBeDamaged() || !IsAbleToAct()) {
		return 0.f;
	}
	DamageAmount = Cast<UAlphaOneInstance>(GetGameInstance())->Battle()->CalcDamage(DamageAmount, DamageCauser, AttributeSet);
	auto hp = AttributeSet->GetHealth() - DamageAmount;
	AttributeSet->InitHealth(hp);
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
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

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
			// const float TriggerRagdollTime = DeathAnimDuration - 0.7f;
			const float TriggerRagdollTime = DeathAnimDuration + 2.0f;

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
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("DESTROY!"));
	// Destroy();
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("IS PENDING KILL: %d!"), IsPendingKill()));
}

void ACharacterBase::SetRagdollPhysics()
{
	// ragdoll is not working properly (pieces are connected)
	Destroy();
	return;
/*
	// building does not have ragdoll
	if (GetType() == EUnitTypes::Building) {
		Destroyed();
		return;
	}

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
*/
}

void ACharacterBase::StopAllAnimMontages()
{
	auto AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Stop(0.0f);
}

AWeaponBase* ACharacterBase::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

bool ACharacterBase::EquipWeapon(AWeaponBase* NewWeapon)
{
	// Use default weapon
	if (!NewWeapon) {
		NewWeapon = DefaultWeaponPtr;
	}

	// the same, no change
	if (CurrentWeapon == NewWeapon) {
		return true;
	}

	// unequip first
	if (CurrentWeapon) {
		CurrentWeapon->DetachFromCharacter();
	}
	CurrentWeapon = NewWeapon;
	CurrentWeapon->AttachToCharacter(this);
	return true;
}
