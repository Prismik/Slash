// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"


UHealthBarComponent::UHealthBarComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHealthBarComponent::BeginPlay() {
	Super::BeginPlay();

	// ...
	
}

void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

