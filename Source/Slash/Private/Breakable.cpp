// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABreakable::ABreakable()
{
 	PrimaryActorTick.bCanEverTick = false;

	geometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(geometryCollection);
	geometryCollection->SetGenerateOverlapEvents(true);
	geometryCollection->SetDamagePropagationData(FGeometryCollectionDamagePropagationData(true, 2.f, 0.f));
	geometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	geometryCollection->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	capsule->SetupAttachment(GetRootComponent());
	capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void ABreakable::BeginPlay() {
	Super::BeginPlay();
	geometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakable::handleBreakEvent);
}

void ABreakable::handleBreakEvent(const FChaosBreakEvent& BreakEvent) {
	if (broken) return;
	broken = true;

	SetLifeSpan(5.0);
	capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	if (breakSound) {
		UGameplayStatics::PlaySoundAtLocation(this, breakSound, GetActorLocation());
	}
	spawnTreasure();
}

void ABreakable::spawnTreasure() {
	UWorld* world = GetWorld();
	if (world && !treasureBlueprintClasses.IsEmpty()) {
		int32 index = UKismetMathLibrary::RandomInteger(treasureBlueprintClasses.Num() * 2);
		if (treasureBlueprintClasses.IsValidIndex(index)) {
			world->SpawnActor<ATreasure>(treasureBlueprintClasses[index], GetActorLocation(), GetActorRotation());
		}
	}
}

void ABreakable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakable::hit_Implementation(const FVector& p) {
	// Handled in handleBreakEvent now
}
