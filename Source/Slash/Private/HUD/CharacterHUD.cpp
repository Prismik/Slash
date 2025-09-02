// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CharacterHUD.h"
#include "HUD/CharacterOverlay.h"
#include "Blueprint/UserWidget.h"
#include "HUD/ItemTooltip.h"

UCharacterOverlay* ACharacterHUD::getOverlay() {
	return mainOverlay;
}

UItemTooltip* ACharacterHUD::getTooltip() {
	return tooltip;
}

void ACharacterHUD::PreInitializeComponents() {
	Super::PreInitializeComponents();
	
	UWorld* world = GetWorld();
	if (world) {
		APlayerController* ctrl = world->GetFirstPlayerController();
		if (ctrl && overlayClass) {
			mainOverlay = CreateWidget<UCharacterOverlay>(ctrl, overlayClass);
			mainOverlay->SetVisibility(ESlateVisibility::Hidden);
			mainOverlay->AddToViewport();
		}

		if (ctrl && tooltipClass) {
			tooltip = CreateWidget<UItemTooltip>(ctrl, tooltipClass);
			tooltip->AddToViewport();
			tooltip->hide();
		}
	}
}