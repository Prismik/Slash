// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class UInputDataConfig;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class AInteractable;
class UInteractor;
class UAnimMontage;
class AWeapon;
class UComboTracker;

UCLASS()
class SLASH_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void addOverlap(AInteractable* interactable);
	void removeOverlap(AInteractable* interactable);

	void setWeapon(AWeapon* weapon);

	UFUNCTION(BlueprintCallable)
	void arm();
	
	UFUNCTION(BlueprintCallable)
	void disarm();
	
	UPROPERTY(VisibleAnywhere, Category="MainCharacter|Movement")
	bool sprintToggled = false;
	
	UPROPERTY(VisibleAnywhere, Category="MainCharacter|Interactor")
	UInteractor* interactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="MainCharacter|Anim")
	ECharacterState state = ECharacterState::ECS_unequipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="MainCharacter|Combo")
	uint8 combo = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MainCharacter|Weapon")
	AWeapon* equippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MainCharacter|Weapon")
	UComboTracker* tracker;

	UPROPERTY(EditDefaultsOnly, Category="MainCharacter|Anim")
	UAnimMontage* armingMontage;

	static const FName HAND_SOCKET;
	static const FName SPINE_SOCKET;
	
	static const FName EQUIP_MONTAGE_SECTION;
	static const FName UNEQUIP_MONTAGE_SECTION;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MainCharacter|EnhancedInput")
	UInputMappingContext* inputMapping;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MainCharacter|EnhancedInput")
	UInputDataConfig* inputActions;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* viewCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainCharacter|Movement")
	float jogSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainCharacter|Movement")
	float sprintSpeed = 650.f;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category="MainCharacter|Anim")
	EActionState actionState = EActionState::EAS_unoccupied;
	
	const float minArmLength = 350.f;
	const float maxArmLength = 750.f;
	
	float targetArmLength;
	
	UFUNCTION()
	void move(const FInputActionValue& Value);
	
	UFUNCTION()
	void look(const FInputActionValue& Value);
	
	UFUNCTION()
	void zoom(const FInputActionValue& Value);

	UFUNCTION()
	void sprint(const FInputActionValue& Value);
	
	UFUNCTION()
	void interact(const FInputActionValue& Value);
	
	UFUNCTION()
	void cycle(const FInputActionValue& Value);

	UFUNCTION()
	void attack(const FInputActionValue& Value);
	
	UFUNCTION()
	void equip(const FInputActionValue& Value);
	
	void playAttackMontage();
	void playArmingMontage(const FName& section);
	void computeTargetSpringArmLength(const float axis);
};
