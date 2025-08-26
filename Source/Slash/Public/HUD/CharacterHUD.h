// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CharacterHUD.generated.h"

class UCharacterOverlay;
class UItemTooltip;

UCLASS()
class SLASH_API ACharacterHUD : public AHUD {
	GENERATED_BODY()

public:
	UCharacterOverlay* getOverlay();
	UItemTooltip* getTooltip();
	
protected:
	virtual void PreInitializeComponents() override;

private:
	UPROPERTY()
	UCharacterOverlay* mainOverlay;
	
	UPROPERTY()
	UItemTooltip* tooltip;
	
	UPROPERTY(EditDefaultsOnly, Category="HUD|Properties")
	TSubclassOf<UCharacterOverlay> overlayClass;

	UPROPERTY(EditDefaultsOnly, Category="HUD|Properties")
	TSubclassOf<UItemTooltip> tooltipClass;
};
