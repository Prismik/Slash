// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hittable.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterTypes.h"
#include "Enemy/FBehavior.h"
#include "Enemy.generated.h"

class UAnimMontage;
class USoundBase;
class UAttributes;
class UHealthBarComponent;
class AController;
struct FDamageEvent;
class AAIController;
class UPawnSensingComponent;
class AEnemyAiController;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHittable {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Enemy|Properties")
	FBehavior aiProperties;
	
	AEnemy();
	
	virtual void hit_Implementation(const FVector& p) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Consts
	static const FName STRUCT_FRONT_SECTION;
	static const FName STRUCK_LEFT_SECTION;
	static const FName STRUCK_RIGHT_SECTION;
	static const FName STRUCK_BACK_SECTION;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enemy")
	EDeathPose deathPose = EDeathPose::EDP_alive;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Anim")
	UAnimMontage* struckMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Anim")
	UAnimMontage* deathMontage;
	
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
	
	void handleDeath();
	FName computeDirectionalStruckSection(const FVector& p);
	void playStruckMontage(const FName& section);
	void playDeathMontage(const FName& section);
};
