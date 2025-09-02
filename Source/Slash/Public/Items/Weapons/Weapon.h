// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboGenerator.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class ACharacter;
class UMetaSoundSource;
class UBoxComponent;

UCLASS()
class SLASH_API AWeapon : public AItem, public IComboGenerator
{
	GENERATED_BODY()

public:
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Type")
	bool twoHanded = false;
	
	virtual void Tick(float DeltaTime) override;
	virtual void interact_Implementation(AMainCharacter* character) override;
	virtual TArray<UAnimMontage*> getCombos() override;
	virtual TArray<float> getDamageMultipliers() override;
	virtual float getBaseDamage() override;
	virtual void setMultiplierIndex(float index) override;
	
	void equip(AMainCharacter* character);
	void unequip(AMainCharacter* character);

	UFUNCTION(BlueprintCallable)
	void attach(ABaseCharacter* toTarget, FName socket);

	UFUNCTION(BlueprintCallable)
	void setBoxCollision(bool enabled);

	UFUNCTION(BlueprintCallable)
	void setBoxExtent(const FVector& halfSize, const FVector& traceSize);

	static const FName TRAIL_START_SOCKET;
	static const FName TRAIL_END_SOCKET;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void boxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void createFields(const FVector& p);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Weapon|Properties")
	UBoxComponent* box;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category = "Weapon|Properties")
	USceneComponent* boxTraceOrigin;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category = "Weapon|Properties")
	USceneComponent* boxTraceDestination;

	UPROPERTY()
	TArray<AActor*> ignoredActors;

	UPROPERTY(EditAnywhere, Category = "Weapon|VFX")
	UParticleSystem* trailParticle;
	
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* trailParticleEmitter;

	UPROPERTY(EditAnywhere)
	float baseDamage = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category = "Weapon|Combo")
	TArray<UAnimMontage*> combos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category = "Weapon|Combo")
	TArray<float> comboDamageMultipliers;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category = "Weapon|Properties")
	FVector boxTraceExtent = FVector(5.f, 5.f, 5.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"), Category = "Weapon|Properties")
	bool boxTraceDebugEnabled;

	void dualBoxTrace(FHitResult& result);
	void boxTrace(const FVector& o, const FVector& d, FHitResult& result);

	float currentMultiplier = 1.f;
};
