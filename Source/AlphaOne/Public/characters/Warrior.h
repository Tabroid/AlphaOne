// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlphaOne.h"
#include "characters/AlphaOnePlayer.h"
#include "Warrior.generated.h"


UCLASS()
class ALPHAONE_API AWarrior : public AAlphaOnePlayer
{
	GENERATED_BODY()

public:
	AWarrior();

	virtual bool Attack();
	virtual void OnAttackEnd(bool Interrupted = false);
};
