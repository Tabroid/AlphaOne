// a header for globals for the whole project
# pragma once

#include "CoreMinimal.h"
#include "characters/CharacterAttributes.h"
#include "AlphaOneGlobals.generated.h"


USTRUCT(BlueprintType)
struct FWeaponSockets
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BindingSockets")
    FName Name = "";
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BindingSockets")
	float Radius = 0;
};

USTRUCT(BlueprintType)
struct FWeaponMontages
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    TArray<FWeaponSockets> Sockets;
};
