// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/FocusIndicatorComponent.h"

#include "HUD/FocusIndicator.h"

UFocusIndicatorComponent::UFocusIndicatorComponent() {
	PrimaryComponentTick.bCanEverTick = true;

}

void UFocusIndicatorComponent::show() {
	if (focus == nullptr) {
		focus = Cast<UFocusIndicator>(GetUserWidgetObject());
	}
	
	focus->SetVisibility(ESlateVisibility::Visible);
}


void UFocusIndicatorComponent::hide() {
	if (focus == nullptr) {
		focus = Cast<UFocusIndicator>(GetUserWidgetObject());
	}
	
	focus->SetVisibility(ESlateVisibility::Hidden);
}

void UFocusIndicatorComponent::BeginPlay() {
	Super::BeginPlay();
	
}
