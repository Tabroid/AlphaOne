#include "AlphaOneBattle.h"
#include "characters/CharacterBase.h"
#include "buildings/BuildingBase.h"
#include "widgets/DamageText.h"
#include "controllers/AlphaOnePlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// helper inline
static inline UCharacterAttributes* _Attr(AActor* Actor) {
    auto Character = Cast<ACharacterBase>(Actor);
    if (IsValid(Character)) {
        return Character->GetAttributes();
    }
    auto Building = Cast<ABuildingBase>(Actor);
    if (IsValid(Building)) {
        return Building->GetAttributes();
    }
    return nullptr;
}

UAlphaOneBattle::UAlphaOneBattle()
{
    // get damage text ref
    static ConstructorHelpers::FObjectFinder<UClass> DamageTextClass(TEXT("WidgetBlueprint'/Game/Blueprint/Widget/WBP_DamageText.WBP_DamageText_C'"));
    if (DamageTextClass.Succeeded()) {
        DamageTextRef = DamageTextClass.Object;
    }
}

UAlphaOneBattle::~UAlphaOneBattle()
{
}

FAttackResult UAlphaOneBattle::InflictDamage(float DamageBase, AActor* Causer, AActor* Taker) const
{
    FAttackResult Result;
    auto CauserAttr = _Attr(Causer);
    auto TakerAttr = _Attr(Taker);
    if (!TakerAttr || !CauserAttr) {
        return Result;
    }

    // calculation
    Result.FinalDamage = DamageBase;
    // @TODO this is just a test
    // Result.DamageType = static_cast<EDamageTypes>(FMath::RandRange(0, static_cast<int32>(EDamageTypes::Max_Types)));

    // inflict damage
    auto NewHealth = TakerAttr->GetHealth() - Result.FinalDamage;
    TakerAttr->InitHealth(NewHealth);
    PopDamageText(Result, Causer, Taker);
    return Result;
}

void UAlphaOneBattle::PopDamageText(const FAttackResult& Result, const AActor* Causer, const AActor* Taker) const
{
    // some safety check
    // if (!DamageTextRef) { return; }
    auto Pawn = Cast<APawn>(Causer);
    if (!IsValid(Pawn)) { return; }
    auto Player = Cast<AAlphaOnePlayerController>(Pawn->GetController());
    if (!IsValid(Player)) { return; }

    auto Position = Taker->GetActorLocation();
    auto Capsule = Cast<UCapsuleComponent>(Taker->GetRootComponent());
    if (Capsule) { Position.Z += Capsule->GetScaledCapsuleHalfHeight(); }

    auto Text = CreateWidget<UDamageText>(Player, DamageTextRef);
    switch (Result.DamageType) {
    case EDamageTypes::Hit:
        Text->PopText(Player, FText::AsNumber(int32(Result.FinalDamage)), Position, FLinearColor::White);
        break;
    case EDamageTypes::Critical:
        Text->PopText(Player, FText::Format(FText::AsCultureInvariant("{:d}!"), int32(Result.FinalDamage)), Position, FLinearColor::Yellow);
        break;
    case EDamageTypes::Missed:
        Text->PopText(Player, FText::AsCultureInvariant("MISS"), Position, FLinearColor::Red);
        break;
    case EDamageTypes::Dodged:
        Text->PopText(Player, FText::AsCultureInvariant("DODGED"), Position, FLinearColor::White);
        break;
    default:
        break;
    }

    Text->AddToViewport(WIDGET_ZORDER_DAMAGE_TEXT);
    // remove for GC
    FTimerHandle TimerHandle;
    Player->GetWorldTimerManager().SetTimer(TimerHandle, Text, &UUserWidget::RemoveFromParent, 2.0f, false);
}
