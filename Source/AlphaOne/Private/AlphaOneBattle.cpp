#include "AlphaOneBattle.h"
#include "characters/CharacterBase.h"
#include "buildings/BuildingBase.h"
#include "widgets/DamageText.h"
#include "controllers/AlphaOnePlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/UserWidget.h"
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

float UAlphaOneBattle::InflictDamage(float DamageBase, AActor* Causer, AActor* Taker) const
{
    auto CauserAttr = _Attr(Causer);
    auto TakerAttr = _Attr(Taker);
    if (!TakerAttr || !CauserAttr) {
        return 0.f;
    }

    float Damage = DamageBase;
    auto NewHealth = TakerAttr->GetHealth() - Damage;
    TakerAttr->InitHealth(NewHealth);
    PopDamageText(Damage, Causer, Taker);
    return Damage;
}

void UAlphaOneBattle::PopDamageText(float Damage, AActor* Causer, AActor* Taker) const
{
    // cast to player controller, do not display damage if it was not caused by a player
    // no more safety checks because it is supposed to be done before calling this function
    auto Pawn = Cast<APawn>(Causer);
    if (!IsValid(Pawn)) { return; }
    auto Controller = Cast<AAlphaOnePlayerController>(Pawn->GetController());
    if (!IsValid(Controller)) { return; }

    FVector2D Position;
    auto Success = UGameplayStatics::ProjectWorldToScreen(Controller, Taker->GetActorLocation(), Position, false);
    // Spawn a user widget
    auto Text = CreateWidget<UDamageText>(Controller, DamageTextRef);
    Text->PopDamage(Damage, Position);
    Text->AddToViewport(WIDGET_ZORDER_DAMAGE_TEXT);

    // remove for GC
    FTimerHandle TimerHandle;
    Controller->GetWorldTimerManager().SetTimer(TimerHandle, Text, &UUserWidget::RemoveFromParent, 2.0f, false);
}