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
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACharacterBase::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACharacterBase::OnStopRunning);
	PlayerInputComponent->BindAction("RunToggle", IE_Pressed, this, &ACharacterBase::OnStartRunningToggle);

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

void ACharacterBase::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
                                  ACharacterBase* InstigatorPawn, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorPawn, DamageCauser);	
}

void ACharacterBase::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	// We only call the BP callback if this is not the initial ability setup
	if (bInitialized) {
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void ACharacterBase::HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	if (bInitialized) {
		OnManaChanged(DeltaValue, EventTags);
	}
}

void ACharacterBase::HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	// Update the character movement's walk speed
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

	if (bInitialized) {
		OnMoveSpeedChanged(DeltaValue, EventTags);
	}
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
	auto bAttacked = Attack();
	SetRunning(false, false);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Attack function returns : %d"), bAttacked));
}

void ACharacterBase::OnStopAttack()
{
	bWantsToAttack = false;
}

void ACharacterBase::SetRunning(bool bNewRunning, bool bToggle)
{
	bWantsToRun = bNewRunning;
	bWantsToRunToggled = bNewRunning && bToggle;
}

void ACharacterBase::OnStartRunning()
{
	OnStopAttack();
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	SetRunning(true, false);
}

void ACharacterBase::OnStartRunningToggle()
{
	OnStopAttack();
	SetRunning(true, true);
}

void ACharacterBase::OnStopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	SetRunning(false, false);
}

bool ACharacterBase::Attack() 
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	auto AnimInstance = GetMesh()->GetAnimInstance();
	if (PlayerController && !bIsAttacking && AnimInstance && NormalAttackMontage) {
		bIsAttacking = true;
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
	bIsAttacking = false;
	if (!interrupted && bWantsToAttack) {
		Attack();
	}
}

bool ACharacterBase::IsSprinting() const
{
	return bWantsToRun;
}