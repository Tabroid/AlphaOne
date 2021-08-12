// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/CharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "weapons/WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
	: DefaultFaction(EUnitFactions::Protagonist)
{
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UAlphaOneAbilitySystem>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	FactionSystemComponent = CreateDefaultSubobject<UFactionComponent>(TEXT("FactionSystemComponent"));

	AttributeSet = CreateDefaultSubobject<UCharacterAttributes>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	CameraBoom->TargetArmLength = DefaultArmLength; // The camera follows at this distance behind the character
	FactionSystemComponent->SetFaction(DefaultFaction);
	if (DefaultWeapon) {
		DefaultWeaponPtr = GetWorld()->SpawnActor<AWeaponBase>(DefaultWeapon, GetMesh()->GetComponentLocation(), GetMesh()->GetComponentRotation());
		DefaultWeaponPtr->SetOwner(Cast<AActor>(this));
		EquipWeapon(DefaultWeaponPtr);
	}
	AttributeSet->InitFromMetaDataTable(Cast<UAlphaOneInstance>(GetGameInstance())->UnitData(), UnitDataRowName);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AttributeSet->RegenOverTime(DeltaTime);
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

void ACharacterBase::SetAction(EUnitActions Action, bool NewState)
{
	if (NewState) {
		AttributeSet->Action |= Action;
	} else {
		AttributeSet->Action &= ~Action;
	}
}

void ACharacterBase::SetStatus(EUnitStatuses Status, bool NewState)
{
	if (NewState) {
		AttributeSet->Status |= Status;
	} else {
		AttributeSet->Status &= ~Status;
	}
}

void ACharacterBase::SetControll(EControllStates Controll, bool NewState)
{
	if (NewState) {
		ControllState |= Controll;
	} else {
		ControllState &= ~Controll;
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
	if ((Controller != nullptr) && (AxisValue != 0.0f))	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void ACharacterBase::MoveRight(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f)) {
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, AxisValue);
	}
}

void ACharacterBase::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * AttributeSet->GetRotationRate() * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::TurnRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * AttributeSet->GetRotationRate() * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::OnStartAttack()
{
	SetControll(EControllStates::WantsToAttack, true);
	SetControll(EControllStates::WantsToSprint, false);
	Attack();
}

void ACharacterBase::OnStopAttack()
{
	SetControll(EControllStates::WantsToAttack, false);
}

void ACharacterBase::OnStartSprinting()
{
	OnStopAttack();
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	SetControll(EControllStates::WantsToSprint, true);
}

void ACharacterBase::OnStopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	SetControll(EControllStates::WantsToSprint, false);
}

bool ACharacterBase::Attack()
{
	if (IsAbleToAct() && CurrentWeapon && !CheckAction(EUnitActions::Attacking)) {
		return CurrentWeapon->Attack();
	}
	return false;
}

void ACharacterBase::OnAttackEnd(bool Interrupted)
{
	SetAction(EUnitActions::Attacking, false);
	if (CurrentWeapon) {
		CurrentWeapon->AttackEnd(Interrupted);
		if (CheckControll(EControllStates::WantsToAttack)  && !Interrupted) {
			Attack();
		}
	}
}

float ACharacterBase::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!CanBeDamaged() || !IsAbleToAct()) {
		return 0.f;
	}
	auto Battle = Cast<UAlphaOneInstance>(GetGameInstance())->Battle();
	auto BattleResult = Battle->InflictDamage(DamageAmount, DamageEvent, DamageCauser, this);

	// @TODO: call on health changed
	if (AttributeSet->GetHealth() <= 0.) {
		Die(BattleResult.FinalDamage, DamageEvent, EventInstigator, DamageCauser);
	}
	return BattleResult.FinalDamage;
}

bool ACharacterBase::IsAbleToAct() const
{
	if (CheckAction(EUnitActions::Dying)				// already dying
		|| IsPendingKill()								// object already destroyed
		|| GetWorld()->GetAuthGameMode<AAlphaOneGameModeBase>() == nullptr) {
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
	SetStatus(EUnitStatuses::Dead);
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

void ACharacterBase::SetJogSpeed(float Value)
{
	JogSpeed = Value;
	// @TODO: should bind this as a delegate
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}
