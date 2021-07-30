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
	return true;
}