// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Griffon.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputDataConfig;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SLASH_API AGriffon : public APawn
{
	GENERATED_BODY()

public:
	AGriffon();
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Griffon|EnhancedInput")
	UInputMappingContext* inputMapping;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Griffon|EnhancedInput")
	UInputDataConfig* inputActions;
private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* capsule;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* viewCamera;
	
	UFUNCTION()
	void move(const FInputActionValue& Value);
	
	UFUNCTION()
	void look(const FInputActionValue& Value);
};
