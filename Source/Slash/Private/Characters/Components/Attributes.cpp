// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/Attributes.h"


UAttributes::UAttributes() {
	PrimaryComponentTick.bCanEverTick = true;

}

float UAttributes::takeDamage(float value) {
	float current = health;
	addHealth(-value);
	float postDamage = health;

	return current - postDamage;
}

void UAttributes::addHealth(float value) {
	health = FMath::Clamp(health + value, 0.0f, maxHealth);
}

void UAttributes::addEnergy(float value) {
	energy = FMath::Clamp(energy + value, 0.0f, maxEnergy);
}

void UAttributes::addSouls(float value) {
	souls += value;
}

float UAttributes::healthPercent() {
	return FMath::Clamp(health / maxHealth, 0.f, 1.f);
}

float UAttributes::energyPercent() {
	return FMath::Clamp(energy / maxEnergy, 0.f, 1.f);
}

bool UAttributes::alive() {
	return health > 0.f;
}

void UAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float delta = energyRegen * DeltaTime;
	energy = FMath::Clamp(energy + delta, 0.f, maxEnergy);
}

