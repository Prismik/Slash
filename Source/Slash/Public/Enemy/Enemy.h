// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboGenerator.h"
#include "Hittable.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy/FBehavior.h"
#include "Enemy.generated.h"

class UFocusIndicatorComponent;
class AAIController;
class ABaseCharacter;
class AController;
class AEnemyAiController;
class AWeapon;
struct FDamageEvent;
class UAnimOrchestrator;
class UAnimMontage;
class UAttributes;
class UEnemyComboTracker;
class UHealthBarComponent;
class UPawnSensingComponent;
class USoundBase;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter, public IHittable, public IComboGenerator {
	GENERATED_BODY()

public:
	AEnemy();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Properties")
	FBehavior aiProperties;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Events")
	void OnFocus();

	UFUNCTION(BlueprintNativeEvent, Category = "Events")
	void OnFocusEnd();

	FVector focusOffset();
	
	virtual void hit_Implementation(const FVector& p, AActor* hitter) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual TArray<UAnimMontage*> getCombos() override;
	virtual TArray<float> getDamageMultipliers() override;
	virtual float getBaseDamage() override;
	virtual void setMultiplierIndex(float index) override;

	void handleAttack();

	static const FName ENEMY_TAG;
	
protected:
	virtual void BeginPlay() override;
	virtual void handleDeath() override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enemy")
	EDeathPose deathPose;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	UHealthBarComponent* healthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "Enemy")
	UFocusIndicatorComponent* focusIndicator;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AEnemyAiController* aiController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category = "Enemy|Combat")
	TArray<UAnimMontage*> combos;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category = "Enemy|Combat")
	TArray<float> comboDamageMultipliers;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category="Enemy|Weapon")
	AWeapon* leftHandWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category="Enemy|Weapon")
	AWeapon* rightHandWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category="Enemy")
	float souls = 5;
	
	void spawnStruckParticles(const FVector& p);
};
