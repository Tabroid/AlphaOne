// Fill out your copyright notice in the Description page of Project Settings.


#include "characters/CharacterBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACharacterBase::ACharacterBase()
: CharacterLevel(1), bInitialized(false)
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

float ACharacterBase::GetBaseMoveSpeed() const
{
	return AttributeSet->GetBaseMoveSpeed();
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
