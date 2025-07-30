// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"

#include "Components/ProgressBar.h"
#include "HUD/HealthBar.h"


UHealthBarComponent::UHealthBarComponent() {
	PrimaryComponentTick.bCanEverTick = true;

}

void UHealthBarComponent::setHealthPercent(float percent) {
	if (healthBar == nullptr) {
		healthBar = Cast<UHealthBar>(GetUserWidgetObject());
	}
	
	if (healthBar) {
		healthBar->progress->SetPercent(percent);
	}
	
	healthBar->SetVisibility(percent == 1.f ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UHealthBarComponent::BeginPlay() {
	Super::BeginPlay();

}

void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

