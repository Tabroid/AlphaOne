// Fill out your copyright notice in the Description page of Project Settings.

#include "buildings/Tower.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "widgets/HealthBar.h"
#include "GameFramework/Character.h"

// Sets default values
ATower::ATower()
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

}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    Cast<UHealthBar>(HealthBar->GetUserWidgetObject())->SetOwnerActor(this);

	CurrentHealth = MaxHealth;
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateHealthBar();
}

void ATower::RotateHealthBar()
{
	if (!PlayerCharacter) return;

	FVector LookDirection = PlayerCharacter->GetActorLocation() - GetActorLocation();

	HealthBar->SetWorldRotation(LookDirection.Rotation());

}

float ATower::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageApplied = FMath::Min(CurrentHealth, DamageApplied);

	CurrentHealth -= DamageApplied;

	return DamageApplied;
}

float ATower::GetHealthPercentage() const
{
	return CurrentHealth / MaxHealth * 100.0f;
}

