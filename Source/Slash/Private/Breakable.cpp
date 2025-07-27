// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ABreakable::ABreakable()
{
 	PrimaryActorTick.bCanEverTick = false;

	geometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(geometryCollection);
	geometryCollection->SetGenerateOverlapEvents(true);
	geometryCollection->SetDamagePropagationData(FGeometryCollectionDamagePropagationData(true, 2.f, 0.f));
	geometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ABreakable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakable::hit_Implementation(const FVector& p) {
	
}

