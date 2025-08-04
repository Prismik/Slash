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

void UAnimOrchestrator::playArming(const FName& section) {
	UAnimInstance* animInst = animated->GetMesh()->GetAnimInstance();
	if (animInst == nullptr || armingMontage == nullptr) return;

	animInst->Montage_Play(armingMontage);
	animInst->Montage_JumpToSection(section);
}

void UAnimOrchestrator::playAttack(UAnimMontage* montage) {
	UAnimInstance* animInst = animated->GetMesh()->GetAnimInstance();
	if (animInst == nullptr || montage == nullptr) return;

	animInst->Montage_Play(montage);
}

void UAnimOrchestrator::playStruck(const FVector& p) {
	UAnimInstance* animInst = animated->GetMesh()->GetAnimInstance();
	if (animInst == nullptr || struckMontage == nullptr) return;
	
	animInst->Montage_Play(struckMontage);
	animInst->Montage_JumpToSection(computeDirectionalStruckSection(p), struckMontage);
}
void UAnimOrchestrator::playDeath(const FName& section) {
	UAnimInstance* animInst = animated->GetMesh()->GetAnimInstance();
	if (animInst == nullptr || deathMontage == nullptr) return;

	animInst->Montage_Play(deathMontage);
	animInst->Montage_JumpToSection(section, deathMontage);
}


void UAnimOrchestrator::BeginPlay() {
	Super::BeginPlay();

	animated = Cast<ACharacter>(GetOwner());
}

FName UAnimOrchestrator::computeDirectionalStruckSection(const FVector& p) {
	const FVector forward = animated->GetActorForwardVector();
	const FVector d = ((p - animated->GetActorLocation()) * FVector(1.f, 1.f, 0.f)).GetSafeNormal();
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
