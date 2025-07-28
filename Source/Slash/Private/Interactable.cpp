// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "Characters/MainCharacter.h"
#include "Components/SphereComponent.h"

AInteractable::AInteractable() {
	PrimaryActorTick.bCanEverTick = true;
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(mesh);
	
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	sphere->SetupAttachment(GetRootComponent());
}

void AInteractable::focus() {
	mesh->SetOverlayMaterial(overlay);
}

void AInteractable::unfocus() {
	mesh->SetOverlayMaterial(nullptr);
}

void AInteractable::sphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AMainCharacter* character = Cast<AMainCharacter>(OtherActor);
	if (!character) return;

	character->addOverlap(this);
}

void AInteractable::sphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	AMainCharacter* character = Cast<AMainCharacter>(OtherActor);
	if (!character) return;

	character->removeOverlap(this);
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay() {
	Super::BeginPlay();
	
	sphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::sphereOverlapBegin);
	sphere->OnComponentEndOverlap.AddDynamic(this, &AInteractable::sphereOverlapEnd);
	
	// cache the overlay for later use with interact range
	overlay = mesh->GetOverlayMaterial();
	mesh->SetOverlayMaterial(nullptr);
}

// Called every frame
void AInteractable::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AInteractable::interact_Implementation(AMainCharacter* character) {
	
}
