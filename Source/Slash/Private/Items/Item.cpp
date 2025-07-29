// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "DrawDebugHelpers.h"

AItem::AItem() {
 	PrimaryActorTick.bCanEverTick = true; // Set this actor to call Tick() every frame.
}

void AItem::BeginPlay() {
	Super::BeginPlay();

}

float AItem::computeSinTransformation() {
	return amplitude * FMath::Sin(runTime * timeConstant);
}

float AItem::computeCosTransformation() {
	return amplitude * FMath::Cos(runTime * timeConstant);
}

void AItem::sphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::sphereOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
}

void AItem::sphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::sphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
}

// Called every frame
void AItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	runTime += DeltaTime;
}