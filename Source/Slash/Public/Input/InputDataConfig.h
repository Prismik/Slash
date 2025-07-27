// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputDataConfig.generated.h"

class UInputAction;

UCLASS()
class SLASH_API UInputDataConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* rotate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* zoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* sprint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* interact;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* cycle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* attack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* heavyAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* equip;
};
