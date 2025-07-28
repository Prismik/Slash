// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Characters/Inventory.h"
#include "Items/Treasure.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/InputDataConfig.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interactable.h"
#include "Characters/Interactor.h"
#include "Animation/AnimMontage.h"
#include "Characters/ComboTracker.h"
#include "Items/Weapons/Weapon.h"

const FName AMainCharacter::HAND_SOCKET(FName("hand_r_socket"));
const FName AMainCharacter::SPINE_SOCKET(FName("spine_socket"));
const FName AMainCharacter::EQUIP_MONTAGE_SECTION(FName("equip"));
const FName AMainCharacter::UNEQUIP_MONTAGE_SECTION(FName("unequip"));

AMainCharacter::AMainCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	// Do not rotate the mesh with the mouse movements
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 360.f;
	springArm->bUsePawnControlRotation = true;
	targetArmLength = springArm->TargetArmLength;

	viewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	viewCamera->SetupAttachment(springArm);

	interactor = CreateDefaultSubobject<UInteractor>(TEXT("Interactor"));
	interactor->character = this;

	tracker = CreateDefaultSubobject<UComboTracker>(TEXT("ComboTracker"));
	
	inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AMainCharacter::addOverlap(AInteractable* interactable) {
	if (!interactable->canInteract) return;
	
	interactor->add(interactable);
}

void AMainCharacter::removeOverlap(AInteractable* interactable) {
	if (!interactable->canInteract) return;
	
	interactor->remove(interactable);
}

void AMainCharacter::collect(ATreasure* treasure) {
	interactor->remove(treasure);
	inventory->currency += treasure->value;
}

void AMainCharacter::setWeapon(AWeapon* weapon) {
	equippedWeapon = weapon;
	tracker->assign(weapon);
	if (weapon == nullptr) {
		state = ECharacterState::ECS_unequipped;
		return;
	}

	interactor->remove(weapon);
	
	if (weapon->twoHanded) {
		state = ECharacterState::ECS_equippedTwoHanded;
	} else {
		state = ECharacterState::ECS_equippedRightHand;
	}
}

void AMainCharacter::arm() {
	if (equippedWeapon == nullptr) return;
	
	equippedWeapon->attach(this->GetMesh(), HAND_SOCKET);
}

void AMainCharacter::disarm() {
	if (equippedWeapon == nullptr) return;
	
	equippedWeapon->attach(this->GetMesh(), SPINE_SOCKET);
}

void AMainCharacter::BeginPlay() {
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = jogSpeed;
}

void AMainCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyEqual(springArm->TargetArmLength, targetArmLength, 0.5f)) {
		springArm->TargetArmLength = FMath::FInterpTo(springArm->TargetArmLength, targetArmLength, DeltaTime, 9.f);
	}
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	subsystem->ClearAllMappings();
	subsystem->AddMappingContext(inputMapping, 0);

	if (!inputActions) return;
	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	input->BindAction(inputActions->move, ETriggerEvent::Triggered, this, &AMainCharacter::move);
	input->BindAction(inputActions->rotate, ETriggerEvent::Triggered, this, &AMainCharacter::look);
	input->BindAction(inputActions->zoom, ETriggerEvent::Triggered, this, &AMainCharacter::zoom);
	input->BindAction(inputActions->sprint, ETriggerEvent::Triggered, this, &AMainCharacter::sprint);
	input->BindAction(inputActions->sprint, ETriggerEvent::Completed, this, &AMainCharacter::sprint);
	input->BindAction(inputActions->interact, ETriggerEvent::Started, this, &AMainCharacter::interact);
	input->BindAction(inputActions->cycle, ETriggerEvent::Started, this, &AMainCharacter::cycle);
	input->BindAction(inputActions->attack, ETriggerEvent::Started, this, &AMainCharacter::attack);
	input->BindAction(inputActions->equip, ETriggerEvent::Started, this, &AMainCharacter::equip);
	input->BindAction(inputActions->jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
}

void AMainCharacter::move(const FInputActionValue& Value) {
	if (actionState != EActionState::EAS_unoccupied) return;
	
	const FVector2D val = Value.Get<FVector2D>();
	const FRotator rotation = GetControlRotation();
	const FRotator yaw = FRotator(0.f, rotation.Yaw, 0.f);
	if (val.Y != 0.f) {
		AddMovementInput(FRotationMatrix(yaw).GetUnitAxis((EAxis::X)), val.Y);
	}

	if (val.X != 0.f) {
		AddMovementInput(FRotationMatrix(yaw).GetUnitAxis((EAxis::Y)), val.X);
	}
}

void AMainCharacter::look(const FInputActionValue& Value) {
	FVector2D val = Value.Get<FVector2D>();
	if (GetController()) {
		AddControllerYawInput(val.X);
		AddControllerPitchInput(val.Y);
	}
}

void AMainCharacter::zoom(const FInputActionValue& Value) {
	float val = Value.Get<float>();
	computeTargetSpringArmLength(val);
}

void AMainCharacter::computeTargetSpringArmLength(const float axis) {
	float unclampedDestination = (-50 * axis) + springArm->TargetArmLength;
	targetArmLength = FMath::Clamp(unclampedDestination, minArmLength, maxArmLength);
}

void AMainCharacter::sprint(const FInputActionValue& Value) {
	bool val = Value.Get<bool>();
	sprintToggled = val;
	GetCharacterMovement()->MaxWalkSpeed = sprintToggled ? sprintSpeed : jogSpeed;
}

void AMainCharacter::interact(const FInputActionValue& Value) {
	bool val = Value.Get<bool>();
	if (!val) { return; }

	interactor->interact();
}

void AMainCharacter::cycle(const FInputActionValue& Value) {
	bool val = Value.Get<bool>();
	if (!val) { return; }

	interactor->cycle();
}

void AMainCharacter::playAttackMontage() {
	UAnimInstance* animInst = GetMesh()->GetAnimInstance();
	UAnimMontage* montage = tracker->getMontage();
	if (animInst == nullptr || montage == nullptr) return;

	animInst->Montage_Play(montage);
}

void AMainCharacter::playArmingMontage(const FName& section) {
	UAnimInstance* animInst = GetMesh()->GetAnimInstance();
	if (animInst == nullptr || armingMontage == nullptr) return;

	animInst->Montage_Play(armingMontage);
	animInst->Montage_JumpToSection(section);
}

void AMainCharacter::attack(const FInputActionValue& Value) {
	if (state == ECharacterState::ECS_unequipped) return;
	if (actionState == EActionState::EAS_equipping) return;
	if (!tracker->canProceed) { return; }
	
	actionState = EActionState::EAS_attacking;
	playAttackMontage();
}

void AMainCharacter::equip(const FInputActionValue& Value) {
	if (actionState != EActionState::EAS_unoccupied) return;
	
	actionState = EActionState::EAS_equipping;
	FName section = state == ECharacterState::ECS_unequipped
		? EQUIP_MONTAGE_SECTION
		: UNEQUIP_MONTAGE_SECTION;
	state = state == ECharacterState::ECS_unequipped
		? ECharacterState::ECS_equippedRightHand
		: ECharacterState::ECS_unequipped;
	playArmingMontage(section);
}
