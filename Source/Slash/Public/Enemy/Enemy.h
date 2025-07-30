// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hittable.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UAnimMontage;
class USoundBase;
class UAttributes;
class UHealthBarComponent;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHittable {
	GENERATED_BODY()

public:
	AEnemy();

	virtual void hit_Implementation(const FVector& p) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	static const FName STRUCT_FRONT_SECTION;
	static const FName STRUCK_LEFT_SECTION;
	static const FName STRUCK_RIGHT_SECTION;
	static const FName STRUCK_BACK_SECTION;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	UAnimMontage* struckMontage;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	UParticleSystem* hitParticle;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess=true), Category = "Enemy")
	UAttributes* attributes;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	UHealthBarComponent* healthBar;
	
	FName computeDirectionalStruckSection(const FVector& p);
	void playStruckMontage(const FName& section);
};
