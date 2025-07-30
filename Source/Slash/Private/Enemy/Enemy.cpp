// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Components/Attributes.h"
#include "HUD/HealthBarComponent.h"

const FName AEnemy::STRUCT_FRONT_SECTION(FName("hitReact_front"));
const FName AEnemy::STRUCK_LEFT_SECTION(FName("hitReact_left"));
const FName AEnemy::STRUCK_RIGHT_SECTION(FName("hitReact_right"));
const FName AEnemy::STRUCK_BACK_SECTION(FName("hitReact_back"));

AEnemy::AEnemy() {
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	attributes = CreateDefaultSubobject<UAttributes>(FName("Attributes"));

	healthBar = CreateDefaultSubobject<UHealthBarComponent>(FName("HealthBar"));
	healthBar->SetupAttachment(GetRootComponent());
}

FName AEnemy::computeDirectionalStruckSection(const FVector& p) {
	const FVector forward = GetActorForwardVector();
	const FVector d = ((p - GetActorLocation()) * FVector(1.f, 1.f, 0.f)).GetSafeNormal();
	const float cos = forward.Dot(d);
	const float theta = FMath::RadiansToDegrees(FMath::Acos(cos));

	FName section;
	if (theta < 45.f) {
		section = STRUCT_FRONT_SECTION;
	} else if (theta < 135.f) {
		bool left = forward.Cross(d).Z < 0.f;
		section = left ? STRUCK_LEFT_SECTION : STRUCK_RIGHT_SECTION;
	} else {
		section = STRUCK_BACK_SECTION;
	}

	return section;
}

void AEnemy::hit_Implementation(const FVector& p) {
	if (hitSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitSound, p);
	}

	if (hitParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(this, hitParticle, p);
	}

	playStruckMontage(computeDirectionalStruckSection(p));
}

void AEnemy::BeginPlay() {
	Super::BeginPlay();
	
}

void AEnemy::playStruckMontage(const FName& section) {
	UAnimInstance* animInst = GetMesh()->GetAnimInstance();
	if (animInst == nullptr || struckMontage == nullptr) return;
	
	animInst->Montage_Play(struckMontage);
	animInst->Montage_JumpToSection(section, struckMontage);
}

void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

