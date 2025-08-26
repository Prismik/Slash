// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ItemTooltipComponent.h"

#include "HUD/ItemTooltip.h"


UItemTooltipComponent::UItemTooltipComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UItemTooltipComponent::show() {
	if (tooltip == nullptr) {
		tooltip = Cast<UItemTooltip>(GetUserWidgetObject());
	}

	tooltip->SetVisibility(ESlateVisibility::Visible);
}

void UItemTooltipComponent::hide() {
	if (tooltip == nullptr) {
		tooltip = Cast<UItemTooltip>(GetUserWidgetObject());
	}

	tooltip->SetVisibility(ESlateVisibility::Hidden);
}