// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacterAnimInstance.h"

#include <filesystem>

#include "Characters/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	mainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	if (mainCharacter) {
		movementComponent = mainCharacter->GetCharacterMovement();
	}
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (movementComponent) {
		groundSpeed = UKismetMathLibrary::VSizeXY(movementComponent->Velocity);
		sprintToggled = mainCharacter->sprintToggled;
		isFalling = movementComponent->IsFalling();
		accelerating = movementComponent->GetCurrentAcceleration().Length() > 0.f;
		characterState = mainCharacter->state;
	}
}