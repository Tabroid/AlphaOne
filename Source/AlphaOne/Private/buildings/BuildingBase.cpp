// Fill out your copyright notice in the Description page of Project Settings.

#include "buildings/BuildingBase.h"
#include "characters/CharacterBase.h"
#include "abilities/AlphaOneAbilitySystem.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "widgets/HealthBar.h"


// Sets default values
ABuildingBase::ABuildingBase()
	: DefaultFaction(EUnitFactions::Defenders)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	AuraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Aura Mesh"));
	AuraMesh->SetupAttachment(BaseMesh);
	AuraMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	HealthBar->SetupAttachment(BaseMesh);

	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UAlphaOneAbilitySystem>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	FactionSystemComponent = CreateDefaultSubobject<UFactionComponent>(TEXT("FactionSystemComponent"));
	FactionSystemComponent->bEditableWhenInherited = true;

	AttributeSet = CreateDefaultSubobject<UCharacterAttributes>(TEXT("AttributeSet"));
	SetType(EUnitTypes::Building);
}

// Called when the game starts or when spawned
void ABuildingBase::BeginPlay()
{
	Super::BeginPlay();
	FactionSystemComponent->SetFaction(DefaultFaction);
	PlayerCharacter = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    // auto health = AbilitySystemComponent->GetAttributeSubobject("AttributeSet")->GetHealth();
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Health: %d!"), health));

    Cast<UHealthBar>(HealthBar->GetUserWidgetObject())->SetAttributeSet(AttributeSet);
}

// Called every frame
void ABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateHealthBar();
}

void ABuildingBase::RotateHealthBar()
{
	if (PlayerCharacter) {
	    FVector LookDirection = PlayerCharacter->GetActorLocation() - GetActorLocation();
	    HealthBar->SetWorldRotation(LookDirection.Rotation());
    }
}

float ABuildingBase::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!CanBeDamaged()) {
		return 0.f;
	}

	
	if (DamageCauser && !GetFactionComponent()->IsEnemy(Cast<AActor>(DamageCauser))) return 0.f;

	auto hp = AttributeSet->GetHealth() - DamageAmount;
	AttributeSet->InitHealth(hp);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Damage: %.0f, Health: %.0f!"), DamageAmount, hp));
	if (hp <= 0.) {
        // @TODO, to implement
		; // Die(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	return DamageAmount;
}

float ABuildingBase::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float ABuildingBase::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float ABuildingBase::GetHealthPercentage() const
{
    return AttributeSet->GetHealth() / AttributeSet->GetMaxHealth();
}

float ABuildingBase::GetMana() const
{
	return AttributeSet->GetMana();
}

float ABuildingBase::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float ABuildingBase::GetManaPercentage() const
{
    return AttributeSet->GetMana() / AttributeSet->GetMaxMana();
}

int32 ABuildingBase::GetLevel() const
{
	return AttributeSet->GetLevel();
}

bool ABuildingBase::SetLevel(int32 NewLevel)
{
	return AttributeSet->SetLevel(NewLevel);
}

void ABuildingBase::SetAction(EUnitActions NewAction, bool State)
{
	if (State) {
		AttributeSet->Action |= NewAction;
	} else {
		AttributeSet->Action &= ~NewAction;
	}
}

void ABuildingBase::SetStatus(EUnitStatuses NewStatus, bool State)
{
	if (State) {
		AttributeSet->Status |= NewStatus;
	} else {
		AttributeSet->Status &= ~NewStatus;
	}
}

void ABuildingBase::SetType(EUnitTypes NewType)
{
	AttributeSet->Type = NewType;
}
