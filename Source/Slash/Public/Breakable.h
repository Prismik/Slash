// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hittable.h"
#include "Breakable.generated.h"

class UGeometryCollectionComponent;
class ATreasure;
class UCapsuleComponent;

UCLASS()
class SLASH_API ABreakable : public AActor, public IHittable
{
	GENERATED_BODY()
	
public:	
	ABreakable();
	virtual void Tick(float DeltaTime) override;
	virtual void hit_Implementation(const FVector& p) override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCapsuleComponent* capsule;
	
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
