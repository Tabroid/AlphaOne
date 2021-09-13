// Fill out your copyright notice in the Description page of Project Settings.

#include "buildings/BuildingBase.h"
#include "characters/CharacterBase.h"
#include "abilities/AlphaOneAbilitySystem.h"
#include "Components/CapsuleComponent.h"
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

    HealthBarUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar UI"));
    HealthBarUI->SetupAttachment(BaseMesh);

    // Create ability system component, and set it to be explicitly replicated
    AbilitySystemComponent = CreateDefaultSubobject<UAlphaOneAbilitySystem>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);

    FactionSystemComponent = CreateDefaultSubobject<UFactionComponent>(TEXT("FactionSystemComponent"));
    FactionSystemComponent->bEditableWhenInherited = true;

    AttributeSet = CreateDefaultSubobject<UCharacterAttributes>(TEXT("AttributeSet"));
    SetType(EUnitTypes::Building);
    DefaultFaction = EUnitFactions::Defenders;
}

// Called when the game starts or when spawned
void ABuildingBase::BeginPlay()
{
    Super::BeginPlay();
    FactionSystemComponent->SetFaction(DefaultFaction);

    // auto health = AbilitySystemComponent->GetAttributeSubobject("AttributeSet")->GetHealth();
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Health: %d!"), health));
    AttributeSet->InitFromMetaDataTable(Cast<UAlphaOneInstance>(GetGameInstance())->UnitData(), UnitDataRowName);
    auto HealthBar = Cast<UHealthBar>(HealthBarUI->GetUserWidgetObject());
    HealthBarUI->SetVisibility(false);
    if (HealthBar) {
        HealthBar->SetAttributeSet(AttributeSet);
        HealthBar->SetOwningComponent(HealthBarUI);
    }
}

// Called every frame
void ABuildingBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AttributeSet->RegenOverTime(DeltaTime);
}

float ABuildingBase::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (!CanBeDamaged()) {
        return 0.f;
    }

    auto Battle = Cast<UAlphaOneInstance>(GetGameInstance())->Battle();
    auto BattleResult = Battle->InflictDamage(DamageAmount, DamageEvent, DamageCauser, this);

    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Damage: %.0f, Health: %.0f!"), BattleResult.FinalDamage, AttributeSet->GetHealth()));
    if (AttributeSet->GetHealth() <= 0.) {
        Die(BattleResult.FinalDamage, DamageEvent, EventInstigator, DamageCauser);
    }
    return BattleResult.FinalDamage;
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


void ABuildingBase::Die(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // @TODO: implement destroy animations
    SetStatus(EUnitStatuses::Dead);
    Destroy();
    return;
}