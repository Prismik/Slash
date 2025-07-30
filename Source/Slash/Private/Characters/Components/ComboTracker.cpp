// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/ComboTracker.h"

#include "Items/Weapons/Weapon.h"


// Sets default values for this component's properties
UComboTracker::UComboTracker() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UComboTracker::assign(AWeapon* weapon) {
	comboGenerator = weapon;
	comboIndex = 0;
}

void UComboTracker::initiate() {
	canProceed = false;
}

void UComboTracker::track() {
	uint8 max = comboGenerator->combos.Num();
	comboIndex = (comboIndex + 1) % max;
	canProceed = true;
}

void UComboTracker::reset() {
	comboIndex = 0;
	canProceed = true;
}

UAnimMontage* UComboTracker::getMontage() {
	if (comboGenerator == nullptr) return nullptr;

	return comboGenerator->combos[comboIndex];
}

void UComboTracker::BeginPlay() {
	Super::BeginPlay();

	// ...
	
}

void UComboTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

