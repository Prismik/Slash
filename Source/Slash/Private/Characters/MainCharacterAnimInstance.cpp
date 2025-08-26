// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacterAnimInstance.h"
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
		float vel = UKismetMathLibrary::VSizeXY(movementComponent->Velocity);
		directionalSpeed = mainCharacter->directionalSpeed * vel;
		groundSpeed = vel;
		sprintToggled = mainCharacter->sprintToggled;
		isFalling = movementComponent->IsFalling();
		accelerating = movementComponent->GetCurrentAcceleration().Length() > 0.f;
		characterState = mainCharacter->state;
		actionState = mainCharacter->actionState;
		dead = mainCharacter->actionState == EActionState::EAS_dead;
		focused = mainCharacter->isFocused();
	}
}