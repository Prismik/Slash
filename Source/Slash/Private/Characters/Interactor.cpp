// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Interactor.h"

#include "Interactable.h"

UInteractor::UInteractor() {
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInteractor::BeginPlay() {
	Super::BeginPlay();

}


// Called every frame
void UInteractor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInteractor::add(AInteractable* interactable) {
	if (interactables.IsEmpty()) {
		interactable->focus();
	}

	interactables.AddUnique(interactable);
}

void UInteractor::remove(AInteractable* interactable) {
	int32 elementIndex = interactables.Find(interactable);
	interactable->unfocus();
	interactables.Remove(interactable);
	
	if (elementIndex == index) {
		index = FMath::Max(0, index - 1);
	} else if (elementIndex < index) {
		index -= 1;
	}

	// Update focus
	if (interactables.IsEmpty()) { return; }
	interactables[index]->focus();
}

// Unfocus current interactable then focus the next one
void UInteractor::cycle() {
	if (interactables.Num() <= 1) { return; }
	
	interactables[index]->unfocus();
	index = (index + 1) % interactables.Num();
	interactables[index]->focus();
}

void UInteractor::interact() {
	if (interactables.IsEmpty()) { return; }
	
	interactables[index]->interact(character);
}

