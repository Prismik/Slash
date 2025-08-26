// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"


int32 ATreasure::getValue() {
	return currency;
}

void ATreasure::interact_Implementation(AMainCharacter* character) {
	Super::interact_Implementation(character);

	if (pickupSound) {
		UGameplayStatics::PlaySoundAtLocation(this, pickupSound, GetActorLocation());
	}
	character->collect(this);
	this->Destroy();
}

void ATreasure::BeginPlay() {
	Super::BeginPlay();
	
	// Currently uniform distribution. Would be nice to have a gaussian instead.
	double multiplier = UKismetMathLibrary::RandomFloatInRange(0.75, 1.5);
	currency = static_cast<int32>(multiplier * baseValue);
}
