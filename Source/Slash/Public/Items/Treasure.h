// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API ATreasure : public AItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly)
	float value = 50;

	virtual void interact_Implementation(AMainCharacter* character) override;
};
