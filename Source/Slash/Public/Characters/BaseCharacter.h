// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UAnimOrchestrator;
class UAttributes;
class UComboTracker;
class UParticleSystem;
class USoundBase;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	bool isAlive();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BaseCharacter|Combat")
	UComboTracker* tracker;
	
	static const FName LEFT_HAND_SOCKET;
	static const FName RIGHT_HAND_SOCKET;
	static const FName DEAD_CHARACTER_TAG;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "BaseCharacter|SFX")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = "BaseCharacter|SFX")
	USoundBase* deathSound;

	UPROPERTY(EditAnywhere, Category = "BaseCharacter|VFX")
	UParticleSystem* hitParticle;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess = "true"), Category = "BaseCharacter")
	UAttributes* attributes;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "BaseCharacter")
	UAnimOrchestrator* orchestrator;


	virtual void handleDeath();
	
	UFUNCTION(BlueprintImplementableEvent, meta = (AllowPrivateAccess = "true"), Category = "Custom")
	void onDeath();
};
