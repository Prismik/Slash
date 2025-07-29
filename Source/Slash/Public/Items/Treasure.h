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
	UPROPERTY(EditAnywhere, Category = "Treasure|Properties")
	int32 baseValue = 0;

	UFUNCTION(BlueprintCallable, Category = "Treasure|Properties")
	int32 getValue();
	
	virtual void interact_Implementation(AMainCharacter* character) override;
	virtual void BeginPlay() override;

private:
	int32 currency = 0;
};
