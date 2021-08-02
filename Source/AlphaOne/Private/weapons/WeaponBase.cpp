// Fill out your copyright notice in the Description page of Project Settings.


#include "weapons/WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	QueryParams.AddIgnoredActor(this);
}

AWeaponBase::~AWeaponBase()
{
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	if (bWantsToSweep) {
		OnSweepComplete();
		RequestAsyncSweep();
	}
}

void AWeaponBase::AttachToCharacter(ACharacterBase* Character)
{
	if (Character) {
		DetachFromCharacter();
		MyCharacter = Character;
		QueryParams.AddIgnoredActor(Character);
	}
}

void AWeaponBase::DetachFromCharacter()
{
	if (MyCharacter) {
		MyCharacter = nullptr;
		// clear always follow adding itself
		QueryParams.ClearIgnoredActors();
		QueryParams.AddIgnoredActor(this);
	}
}

bool AWeaponBase::Attack()
{
	// safety check
	if (!MyCharacter || !MyCharacter->GetMesh() || !AttackMontages.Num()) {
		return false;
	}

	auto AnimInstance = MyCharacter->GetMesh()->GetAnimInstance();

	// reset combo for a new circle
	if ((AttackCombo >= AttackMontages.Num()) || (AttackCombo < 0)) {
		AttackCombo = 0;
	}
	auto AttackMontage = AttackMontages[AttackCombo].Montage;
	MyCharacter->SetAction(EUnitActions::Attacking, true);
	AnimInstance->Montage_Play(AttackMontage, AttackRate);
	// FOnMontageEnded MontageEndDelegate;
    // MontageEndDelegate.BindUObject(this, &ACharacterBase::OnPlayAttackEnd);
	// AnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
	return true;
}

FVector AWeaponBase::GetSocketLocation(FName name) const
{
	if (CollisionMesh) {
		return CollisionMesh->GetSocketLocation(name);
	} else if (MyCharacter && MyCharacter->GetMesh()) {
		return MyCharacter->GetMesh()->GetSocketLocation(name);
	} else {
		return FVector();
	}
}

FRotator AWeaponBase::GetSocketRotation(FName name) const
{
	if (CollisionMesh) {
		return CollisionMesh->GetSocketRotation(name);
	} else if (MyCharacter && MyCharacter->GetMesh()) {
		return MyCharacter->GetMesh()->GetSocketRotation(name);
	} else {
		return FRotator();
	}
}

const TArray<FWeaponSockets>& AWeaponBase::GetCollisionSockets() const
{
	// intentionally give error if AttackCombo is out of array
	int32 Index = (AttackCombo >= AttackMontages.Num()) ? 0 : AttackCombo;
	return AttackMontages[Index].Sockets;
}

TArray<FWeaponSockets>& AWeaponBase::GetCollisionSockets()
{
	// intentionally give error if AttackCombo is out of array
	int32 Index = (AttackCombo >= AttackMontages.Num()) ? 0 : AttackCombo;
	return AttackMontages[Index].Sockets;
}

void AWeaponBase::OnSweepBegin()
{
	if (!IsValid(MyCharacter)) {
		return;
	}

    for (auto &[Name, Radius, Location_Prev, Rotation_Prev, QueryHandle_Prev] : GetCollisionSockets()) {
		Location_Prev = GetSocketLocation(Name);
		Rotation_Prev = GetSocketRotation(Name);
		QueryHandle_Prev._Data.FrameNumber = 0;
    }
    HitActors.Empty();

	bWantsToSweep = true;
	RequestAsyncSweep();
}

// use sweep results
void AWeaponBase::OnSweepComplete()
{
    if (!IsValid(MyCharacter) || !IsValid(GetWorld())) {
        return;
    }
	auto World = GetWorld();
    auto FactionComp = MyCharacter->GetFactionComponent();

    for (auto &[Name, Radius, Location_Prev, Rotation_Prev, QueryHandle_Prev] : GetCollisionSockets()) {
        if (!World->QueryTraceData(QueryHandle_Prev, SweepResult)) {
            continue;
        }
        // Clear out handle
        QueryHandle_Prev._Data.FrameNumber = 0;
        for (auto &Hit: SweepResult.OutHits) {
            auto Actor = Hit.GetActor();
            if (!FactionComp->IsEnemy(Actor)) {
                continue;
            }
            if (!HitActors.Contains(Actor)) {
                HitActors.Add(Actor);
                FPointDamageEvent DamageEvent(GetDamage(), Hit, Hit.Normal, GetDamageType());
                Actor->TakeDamage(GetDamage(), DamageEvent, MyCharacter->GetController(), MyCharacter);
            }
        }
    }
}

// request async sweep, data is available in NEXT TICK
void AWeaponBase::RequestAsyncSweep()
{
    if (!IsValid(GetWorld())) {
        return;
    }

    for (auto &[Name, Radius, Location_Prev, Rotation_Prev, QueryHandle_Prev] : GetCollisionSockets()) {
		if (QueryHandle_Prev._Data.FrameNumber != 0) {
			continue;
		}
        auto SocketLocation = GetSocketLocation(Name);
        auto SocketRotation = GetSocketRotation(Name);
        QueryHandle_Prev = GetWorld()->AsyncSweepByChannel(EAsyncTraceType::Multi,
                                                           Location_Prev, SocketLocation, FQuat(SocketRotation - Rotation_Prev),
                                                           ECC_PhysicsBody,
                                                           FCollisionShape::MakeSphere(Radius),
                                                           QueryParams,
                                                           FCollisionResponseParams::DefaultResponseParam,
                                                           nullptr, 0);
        Location_Prev = SocketLocation;
        Rotation_Prev = SocketRotation;
    }
}

void AWeaponBase::OnSweepEnd()
{
	OnSweepComplete();
	bWantsToSweep = false;
}
