// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboGenerator.h"
#include "Hittable.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterTypes.h"
#include "Enemy/FBehavior.h"
#include "Enemy.generated.h"

class UAnimOrchestrator;
class UAnimMontage;
class USoundBase;
class UAttributes;
class UHealthBarComponent;
class AController;
struct FDamageEvent;
class AAIController;
class UPawnSensingComponent;
class AEnemyAiController;
class UEnemyComboTracker;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHittable, public IComboGenerator {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Properties")
	FBehavior aiProperties;
	
	UPROPERTY(BlueprintReadOnly, Category="Enemy|Combat")
	UEnemyComboTracker* tracker;
	
	AEnemy();
	
	virtual void hit_Implementation(const FVector& p) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual TArray<UAnimMontage*> getCombos() override;

	void handleAttack();

	// Consts
	static const FName STRUCT_FRONT_SECTION;
	static const FName STRUCK_LEFT_SECTION;
	static const FName STRUCK_RIGHT_SECTION;
	static const FName STRUCK_BACK_SECTION;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Enemy")
	UAnimOrchestrator* orchestrator;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enemy")
	EDeathPose deathPose = EDeathPose::EDP_death1;
	
	UPROPERTY(EditAnywhere, Category = "Enemy")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	UParticleSystem* hitParticle;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess = "true"), Category = "Enemy")
	UAttributes* attributes;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	UHealthBarComponent* healthBar;
	
	UPROPERTY()
	AEnemyAiController* aiController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category = "Enemy|Combat")
	TArray<UAnimMontage*> combos;
	
	void handleDeath();
};
