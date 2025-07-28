// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"

#include "DebugUtilities.h"
#include "Characters/Interactor.h"
#include "Characters/MainCharacter.h"


void ATreasure::interact_Implementation(AMainCharacter* character) {
	Super::interact_Implementation(character);

	UDebugUtilities::print(FString("Called interact on coin"));
	character->collect(this);
	this->Destroy();
}
