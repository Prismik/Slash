// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/Attributes.h"


UAttributes::UAttributes() {
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributes::BeginPlay() {
	Super::BeginPlay();

}

void UAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

float UAttributes::healthPercent() {
	return FMath::Clamp(health / maxHealth, 0.f, 1.f);
}

float UAttributes::energyPercent() {
	return FMath::Clamp(energy / maxEnergy, 0.f, 1.f);
}

