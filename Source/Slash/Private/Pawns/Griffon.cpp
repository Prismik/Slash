// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Griffon.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/InputDataConfig.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AGriffon::AGriffon() {
	PrimaryActorTick.bCanEverTick = true; // Set this pawn to call Tick() every frame.
	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(capsule);
	
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(GetRootComponent());
	
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 360.f;

	viewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	viewCamera->SetupAttachment(springArm);
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AGriffon::BeginPlay() {
	Super::BeginPlay();
	
}

void AGriffon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGriffon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* playerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	subsystem->ClearAllMappings();
	subsystem->AddMappingContext(inputMapping, 0);

	if (!inputActions) return;
	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	input->BindAction(inputActions->move, ETriggerEvent::Triggered, this, &AGriffon::move);
	input->BindAction(inputActions->rotate, ETriggerEvent::Triggered, this, &AGriffon::look);
}

void AGriffon::move(const FInputActionValue& Value) {
	FVector2D val = Value.Get<FVector2D>();
	if (GetController()) {
		auto movement = FVector(val.X, val.Y, 0.f) * GetActorForwardVector();
		AddMovementInput(GetActorForwardVector(), val.Y);
	}
}

void AGriffon::look(const FInputActionValue& Value) {
	FVector2D val = Value.Get<FVector2D>();
	if (GetController()) {
		AddControllerYawInput(val.X);
		AddControllerPitchInput(val.Y);
	}
}