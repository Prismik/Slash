// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hittable.h"
#include "Breakable.generated.h"

class UGeometryCollectionComponent;

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

private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* geometryCollection;
};
