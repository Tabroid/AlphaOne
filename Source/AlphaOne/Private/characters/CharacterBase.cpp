// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/CharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	auto AnimInstance = GetMesh()->GetAnimInstance();
	if (PlayerController && !(GetAction() & EUnitActions::Attacking) && AnimInstance && NormalAttackMontage) {
		SetAction(EUnitActions::Attacking, true);
		float PlayTime = AnimInstance->Montage_Play(NormalAttackMontage, NormalAttackRate);
		FOnMontageEnded MontageBlendOutDelegate;
    	MontageBlendOutDelegate.BindUObject(this, &ACharacterBase::OnPlayAttackEnd);
		AnimInstance->Montage_SetEndDelegate(MontageBlendOutDelegate);
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
