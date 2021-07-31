// Fill out your copyright notice in the Description page of Project Settings.


#include "weapons/WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	Super::Tick(DeltaTime);
}

void AWeaponBase::AttachToCharacter(ACharacterBase* Character)
{
	if (Character) {
		DetachFromCharacter();
		MyCharacter = Character;
	}
}

void AWeaponBase::DetachFromCharacter()
{
	if (MyCharacter) {
		MyCharacter = nullptr;
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
	return AttackMontages[AttackCombo].Sockets;
}