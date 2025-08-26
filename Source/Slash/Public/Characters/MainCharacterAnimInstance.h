// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "MainCharacterAnimInstance.generated.h"

class AMainCharacter;
class UCharacterMovementComponent;

UCLASS()
class SLASH_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	AMainCharacter* mainCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* movementComponent;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float groundSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool sprintToggled;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool isFalling;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool accelerating = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	FVector2D directionalSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = ActionState)
	bool dead = false;

	UPROPERTY(BlueprintReadOnly, Category = ActionState)
	bool focused = false;
	
	UPROPERTY(BlueprintReadOnly, Category = State)
	ECharacterState characterState;

	UPROPERTY(BlueprintReadOnly, Category = ActionState)
	EActionState actionState;
};
