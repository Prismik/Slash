// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hittable.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UAnimMontage;
class USoundBase;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHittable {
	GENERATED_BODY()

public:
	AEnemy();

	virtual void hit(const FVector& p) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	static const FName STRUCT_FRONT_SECTION;
	static const FName STRUCK_LEFT_SECTION;
	static const FName STRUCK_RIGHT_SECTION;
	static const FName STRUCK_BACK_SECTION;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Anim")
	UAnimMontage* struckMontage;

	UPROPERTY(EditAnywhere, Category = "Enemy|Audio")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = "Enemy|VFX")
	UParticleSystem* hitParticle;
	
	FName computeDirectionalStruckSection(const FVector& p);
	void playStruckMontage(const FName& section);
};
