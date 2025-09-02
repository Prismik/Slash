// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Hittable.h"
#include "CharacterTypes.h"
#include "MainCharacter.generated.h"

class AEnemy;
class ASoul;
class UItemTooltip;
class USphereComponent;
class ABaseCharacter;
class UCharacterOverlay;
class UAnimOrchestrator;
struct FInputActionValue;
class UInputDataConfig;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class AInteractable;
class UInteractor;
class UAnimMontage;
class AWeapon;
class UComboTracker;
class UInventory;
class ATreasure;
class UAttributes;

UCLASS()
class SLASH_API AMainCharacter : public ABaseCharacter, public IHittable
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category="MainCharacter|Movement")
	bool sprintToggled = false;
	
	UPROPERTY(VisibleAnywhere, Category="MainCharacter|Movement")
	FVector2D directionalSpeed = FVector2D::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category="MainCharacter|Combat")
	AEnemy* focusedTarget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="MainCharacter|Anim")
	ECharacterState state = ECharacterState::ECS_unequipped;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="MainCharacter|Anim")
	EActionState actionState = EActionState::EAS_unoccupied;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MainCharacter|Weapon")
	AWeapon* equippedWeapon;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="MainCharacter|Inventory")
	UInventory* inventory;
	
	static const FName SPINE_SOCKET;
	static const FName EQUIP_MONTAGE_SECTION;
	static const FName UNEQUIP_MONTAGE_SECTION;
	static const FName MAIN_CHARACTER_TAG;
	
	AMainCharacter();

	virtual void hit_Implementation(const FVector& p, AActor* hitter) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Jump() override;
	virtual void handleDeath() override;
	bool isFocused();
	void addOverlap(AInteractable* interactable);
	void removeOverlap(AInteractable* interactable);
	void collect(ATreasure* treasure);
	void collect(ASoul* soul);
	void setWeapon(AWeapon* weapon);

	UFUNCTION(BlueprintCallable)
	void arm();
	
	UFUNCTION(BlueprintCallable)
	void disarm();

protected:
	UPROPERTY(EditAnywhere, Category = "BaseCharacter|SFX")
	USoundBase* noEnergySound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MainCharacter|EnhancedInput")
	UInputMappingContext* inputMapping;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MainCharacter|EnhancedInput")
	UInputDataConfig* inputActions;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* viewCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainCharacter|Movement")
	float walkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainCharacter|Movement")
	float jogSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainCharacter|Movement")
	float sprintSpeed = 650.f;
	
	virtual void BeginPlay() override;
	
private:
	const float minArmLength = 350.f;
	const float maxArmLength = 750.f;
	
	float targetArmLength;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category="MainCharacter|Interactor")
	UInteractor* interactor;

	UPROPERTY()
	UCharacterOverlay* overlay;

	UPROPERTY()
	UItemTooltip* tooltip;
	
	UFUNCTION()
	void move(const FInputActionValue& Value);
	
	UFUNCTION()
	void endMove(const FInputActionValue& Value);
	
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

	UFUNCTION()
	void focus(const FInputActionValue& Value);

	UFUNCTION()
	void dodge(const FInputActionValue& Value);
	
	void computeTargetSpringArmLength(const float axis);
	bool canAttack();
	bool canDodge();
	void initializeOverlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="MainCharacter|Movement")
	EInputDirection inputDirection = EInputDirection::EID_none;
	
	// Focus on targets
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="MainCharacter|Combat")
	USphereComponent* focusSphere;
	
	UPROPERTY()
	TArray<AEnemy*> focusableEnemies;

	UPROPERTY(VisibleInstanceOnly, Category="MainCharacter|Movement")
	FVector focusVector = FVector::ZeroVector;
	
	EFocusMode focusMode = EFocusMode::EFM_unfocused;
	void focus(AEnemy* target);
	
	UFUNCTION()
	virtual void sphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void sphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
