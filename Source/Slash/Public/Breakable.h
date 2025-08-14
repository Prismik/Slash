// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hittable.h"
#include "Breakable.generated.h"

class UGeometryCollectionComponent;
class ATreasure;
class UCapsuleComponent;
struct FChaosBreakEvent;

UCLASS()
class SLASH_API ABreakable : public AActor, public IHittable
{
	GENERATED_BODY()
	
public:	
	ABreakable();
	virtual void Tick(float DeltaTime) override;
	virtual void hit_Implementation(const FVector& p, AActor* hitter) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* capsule;

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void handleBreakEvent(const FChaosBreakEvent& BreakEvent);
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Breakable|Audio")
	USoundBase* breakSound;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category="Breakable|Properties")
	bool broken = false;
	
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* geometryCollection;

	UPROPERTY(EditDefaultsOnly, Category="Breakable|treasure")
	TArray<TSubclassOf<ATreasure>> treasureBlueprintClasses;

	void spawnTreasure();
};
