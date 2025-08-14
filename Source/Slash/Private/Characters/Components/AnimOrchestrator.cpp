// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/AnimOrchestrator.h"
#include "GameFramework/Character.h"

const FName UAnimOrchestrator::STRUCT_FRONT_SECTION(FName("hitReact_front"));
const FName UAnimOrchestrator::STRUCK_LEFT_SECTION(FName("hitReact_left"));
const FName UAnimOrchestrator::STRUCK_RIGHT_SECTION(FName("hitReact_right"));
const FName UAnimOrchestrator::STRUCK_BACK_SECTION(FName("hitReact_back"));

UAnimOrchestrator::UAnimOrchestrator() {
	PrimaryComponentTick.bCanEverTick = false;

}

void UAnimOrchestrator::playArming(FName section) {
	playMontage(armingMontage, &section);
}

void UAnimOrchestrator::playAttack(UAnimMontage* montage) {
	playMontage(montage);
}

void UAnimOrchestrator::playStruck(const FVector& p, const FVector& o) {
	FName section = computeDirectionalStruckSection(p, o);
	playMontage(struckMontage, &section);
}

EDeathPose UAnimOrchestrator::playDeath() {
	const uint8 rng = playMontage(deathMontage, deathMontageSections);
	return rng >= static_cast<uint8>(EDeathPose::EDP_max)
		? EDeathPose::EDP_max
		: static_cast<EDeathPose>(rng);
}

void UAnimOrchestrator::BeginPlay() {
	Super::BeginPlay();

	animated = Cast<ACharacter>(GetOwner());
}

FName UAnimOrchestrator::computeDirectionalStruckSection(const FVector& p, const FVector& o) {
	const FVector forward = animated->GetActorForwardVector();
	const FVector d = ((o - animated->GetActorLocation()) * FVector(1.f, 1.f, 0.f)).GetSafeNormal();
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

void UAnimOrchestrator::playMontage(UAnimMontage* montage, FName* section) {
	UAnimInstance* animInst = animated->GetMesh()->GetAnimInstance();
	if (animInst == nullptr || montage == nullptr) return;
	animInst->Montage_Play(montage);
	
	if (!section) return;
	animInst->Montage_JumpToSection(*section, deathMontage);
}

void UAnimOrchestrator::playMontage(UAnimMontage* montage) {
	playMontage(montage, nullptr);
}

uint8 UAnimOrchestrator::playMontage(UAnimMontage* montage, const TArray<FName>& sections) {
	if (sections.IsEmpty()) return 0;
	const uint32 rng = FMath::RandRange(0, sections.Num() - 1);
	FName section = sections[rng];
	playMontage(montage, &section);
	return rng;
}
