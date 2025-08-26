// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

ASoul::ASoul() {
	rotationSpeed = 20.f;
	canInteract = false;
	autoInteract = true;
}

float ASoul::getValue() {
	return value;
}

void ASoul::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (state == EItemState::EIS_hovering) {
		AddActorWorldOffset(FVector(0.f,0.f, computeSinTransformation()));
	}
	
	// Roll: X, Pitch: y, Yaw: Z
	if (rotationSpeed != 0.f) {
		AddActorLocalRotation(FRotator(0.f, rotationSpeed * DeltaTime, 0.f));
	}

	magnetize(DeltaTime, 0.75f);
}

void ASoul::interact_Implementation(AMainCharacter* character) {
	Super::interact_Implementation(character);

	state = EItemState::EIS_equipped;
	rotationSpeed = 0.f;

	magnetTarget = character;
}

void ASoul::magnetize(float dt, float speed) {
	if (!magnetTarget) return;

	if (magnetizeSound && !magnetized) {
		magnetized = true;
		UGameplayStatics::PlaySoundAtLocation(this, magnetizeSound, GetActorLocation());
	}
	
	float distance = (GetActorLocation() - magnetTarget->GetActorLocation()).Length();
	if (distance < 33.f) {
		pickup();
	}

	FVector translate = FMath::InterpExpoOut(GetActorLocation(), magnetTarget->GetActorLocation(), speed * dt);
	SetActorLocation(translate);
}

void ASoul::spawnPickupEffect() {
	if (!pickupEffect) return;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, pickupEffect, GetActorLocation());
}

void ASoul::pickup() {
	if (pickupSound) {
		UGameplayStatics::PlaySoundAtLocation(this, pickupSound, GetActorLocation());
	}
	magnetTarget->collect(this);
	spawnPickupEffect();
	this->Destroy();
}
