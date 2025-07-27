// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class ACharacter;
class UMetaSoundSource;
class UBoxComponent;

UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Type")
	bool twoHanded = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Combo")
	TArray<UAnimMontage*> combos;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Audio")
	USoundBase* equipSound;
	
	virtual void Tick(float DeltaTime) override;

	void equip(AMainCharacter* character);
	void unequip(AMainCharacter* character);

	void attach(UMeshComponent* toTarget, FName socket);
	virtual void interact(AMainCharacter* character) override;

	UFUNCTION(BlueprintCallable)
	void setBoxCollision(bool enabled);
	
	static const FName TRAIL_START_SOCKET;
	static const FName TRAIL_END_SOCKET;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void sphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void sphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void boxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void createFields(const FVector& p);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Weapon|Properties")
	UBoxComponent* box;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* boxTraceOrigin;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* boxTraceDestination;

	TArray<AActor*> ignoredActors;

	UPROPERTY(EditAnywhere, Category = "Weapon|VFX")
	UParticleSystem* trailParticle;
	
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* trailParticleEmitter;
};
