// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboGenerator.h"
#include "Hittable.h"
#include "Characters/BaseCharacter.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterTypes.h"
#include "Enemy/FBehavior.h"
#include "Enemy.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Properties")
	FBehavior aiProperties;
	
	UPROPERTY(BlueprintReadOnly, Category="Enemy|Combat")
	UEnemyComboTracker* tracker;
	
	AEnemy();
	
	virtual void hit_Implementation(const FVector& p) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual TArray<UAnimMontage*> getCombos() override;

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
	
	UPROPERTY()
	AEnemyAiController* aiController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category = "Enemy|Combat")
	TArray<UAnimMontage*> combos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category="Enemy|Weapon")
	AWeapon* leftHandWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category="Enemy|Weapon")
	AWeapon* rightHandWeapon;
	
	void spawnStruckParticles(const FVector& p);
};
