// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "Components/ActorComponent.h"
#include "AnimOrchestrator.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UAnimOrchestrator : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAnimOrchestrator();

	void playArming(FName section);
	void playAttack(UAnimMontage* montage);
	/// Play struck animation from a hit point
	void playStruck(const FVector& p);
	/// Play a random death animation and return the chosen pose
	EDeathPose playDeath();
	
	// Consts
	static const FName STRUCT_FRONT_SECTION;
	static const FName STRUCK_LEFT_SECTION;
	static const FName STRUCK_RIGHT_SECTION;
	static const FName STRUCK_BACK_SECTION;

protected:
	virtual void BeginPlay() override;
	
private:
	ACharacter* animated;
	
	FName computeDirectionalStruckSection(const FVector& p);
	void playMontage(UAnimMontage* montage, FName* section);
	void playMontage(UAnimMontage* montage);
	uint8 playMontage(UAnimMontage* montage, const TArray<FName>& sections);
	
	// Montages
	UPROPERTY(EditDefaultsOnly, Category="Anim montages")
	UAnimMontage* armingMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim montages")
	UAnimMontage* struckMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim montages|Death")
	UAnimMontage* deathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim montages|Death")
	TArray<FName> deathMontageSections;
};
