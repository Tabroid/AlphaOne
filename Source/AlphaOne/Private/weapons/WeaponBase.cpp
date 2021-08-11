// Fill out your copyright notice in the Description page of Project Settings.


#include "weapons/WeaponBase.h"
#include "DrawDebugHelpers.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	QueryParams.AddIgnoredActor(this);
	SweepDelegate.BindUObject(this, &AWeaponBase::OnSweepComplete);
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
	if (WeaponMesh) {
		return WeaponMesh->GetSocketLocation(name);
	} else if (MyCharacter && MyCharacter->GetMesh()) {
		return MyCharacter->GetMesh()->GetSocketLocation(name);
	} else {
		return FVector();
	}
}

FRotator AWeaponBase::GetSocketRotation(FName name) const
{
	if (WeaponMesh) {
		return WeaponMesh->GetSocketRotation(name);
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
		QueryHandle_Prev = FTraceHandle();
    }
    HitActors.Empty();

	bWantsToSweep = true;
	for (int32 i = 0; i < GetCollisionSockets().Num(); ++i) {
		RequestAsyncSweep(i);
	}
}

// use sweep results
void AWeaponBase::OnSweepComplete(const FTraceHandle &Handle, FTraceDatum &Data)
{
    if (!IsValid(MyCharacter) || !IsValid(GetWorld())) {
        return;
    }
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Sweep complete: %d!"), Data.UserData));
	auto World = GetWorld();
    auto FactionComp = MyCharacter->GetFactionComponent();

	int32 Index = Data.UserData;
	for (auto &Hit: Data.OutHits) {
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

	if (bWantsToSweep) {
		RequestAsyncSweep(Index);
	}
	// SweepDebugDrawings(Data);
}

void AWeaponBase::SweepDebugDrawings(const FTraceDatum &TraceData, bool Persist, float LifeTime, uint8 Depth, float Thickness)
{
	FColor Color = TraceData.OutHits.Num() ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), TraceData.Start, TraceData.CollisionParams.CollisionShape.GetSphereRadius(), 6,
	                Color, Persist, LifeTime, Depth, Thickness);
	DrawDebugLine(GetWorld(), TraceData.Start, TraceData.End, Color, Persist, LifeTime, Depth, Thickness);
}

// request async sweep, data is available in NEXT TICK
void AWeaponBase::RequestAsyncSweep(int32 Index)
{
    if (!IsValid(GetWorld())) {
        return;
    }

	auto &[Name, Radius, Location_Prev, Rotation_Prev, QueryHandle_Prev] = GetCollisionSockets()[Index];
	auto SocketLocation = GetSocketLocation(Name);
	auto SocketRotation = GetSocketRotation(Name);
	QueryHandle_Prev = GetWorld()->AsyncSweepByChannel(EAsyncTraceType::Multi,
													   Location_Prev, SocketLocation, FQuat(SocketRotation - Rotation_Prev),
													   ECC_PhysicsBody,
													   FCollisionShape::MakeSphere(Radius),
													   QueryParams,
													   FCollisionResponseParams::DefaultResponseParam,
													   &SweepDelegate, Index);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
	//  								 FString::Printf(TEXT("Sweep query: %d! Start: (%.02f, %.02f, %.02f), End: (%.02f, %.02f, %.02f)"), Index,
	// 								                 Location_Prev.X, Location_Prev.Y, Location_Prev.Z, SocketLocation.X, SocketLocation.Y, SocketLocation.Z));

	Location_Prev = SocketLocation;
	Rotation_Prev = SocketRotation;
}

void AWeaponBase::OnSweepEnd()
{
	bWantsToSweep = false;
}
