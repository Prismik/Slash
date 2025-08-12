// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Characters/Components/Attributes.h"
#include "Kismet/GameplayStatics.h"


const FName ABaseCharacter::LEFT_HAND_SOCKET(FName("hand_l_socket"));
const FName ABaseCharacter::RIGHT_HAND_SOCKET(FName("hand_r_socket"));

// Sets default values
ABaseCharacter::ABaseCharacter() {
	attributes = CreateDefaultSubobject<UAttributes>(FName("Attributes"));
	
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay() {
	Super::BeginPlay();
	
}

void ABaseCharacter::handleDeath() {
	if (deathSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), deathSound, GetActorLocation());
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

