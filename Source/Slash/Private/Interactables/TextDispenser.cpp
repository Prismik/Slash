// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/TextDispenser.h"

#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"

ATextDispenser::ATextDispenser() {
	PrimaryActorTick.bCanEverTick = true;
	
	textRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	textRender->SetupAttachment(GetRootComponent());
	textRender->SetTextRenderColor(FColor::Red);
	textRender->SetHorizontalAlignment(EHTA_Center);
	textRender->SetWorldSize(32.f);
	textRender->SetVisibility(false);
	
	autoInteract = true;
	canInteract = false;
}

void ATextDispenser::show() {
	if (once && shownTime > 0) return;
	
	state = EDispenserState::EDS_showing;
	shownTime += 1;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), appearSound, GetActorLocation());
	
	currentlyTypedText.Empty();
	textRender->SetText(FText::FromString(currentlyTypedText));
	textRender->SetVisibility(true);
	GetWorldTimerManager().SetTimer(typingTimer, this, &ATextDispenser::typeLetter, letterTypeDelay());
	targetScale = 1.f;
}

void ATextDispenser::hide() {
	GetWorldTimerManager().ClearTimer(typingTimer);
	state = EDispenserState::EDS_hiding;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), disappearSound, GetActorLocation());
	
	index = -1;
	targetScale = 0.f;
}

void ATextDispenser::interact_Implementation(AMainCharacter* character) {
	Super::interact_Implementation(character);

	show();
}

void ATextDispenser::sphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::sphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	hide();
}

void ATextDispenser::BeginPlay() {
	Super::BeginPlay();

	shownTime = 0;
}

void ATextDispenser::typeLetter() {
	index += 1;
	if (index >= text.Len()) return;
	
	TCHAR next = text[index];
	if (next == '<') {
		index += 4;
		currentlyTypedText.Append(FString("<br>"));
	}
	next = text[index];
	currentlyTypedText.AppendChar(next);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), letterTypeSound, GetActorLocation());
	textRender->SetText(FText::FromString(currentlyTypedText));
	GetWorldTimerManager().SetTimer(typingTimer, this, &ATextDispenser::typeLetter, letterTypeDelay());
}

void ATextDispenser::showingTick(float dt) {
	if (updateProgress(dt)) {
		state = EDispenserState::EDS_active;
	}
}

void ATextDispenser::hidingTick(float dt) {
	if (updateProgress(dt)) {
		state = EDispenserState::EDS_dormant;
		currentlyTypedText.Empty();
		textRender->SetText(FText::FromString(currentlyTypedText));
		textRender->SetVisibility(false);
	}
}

float ATextDispenser::letterTypeDelay() {
	return averageTypingSpeed * FMath::RandRange(0.5f, 1.5f);
}

bool ATextDispenser::updateProgress(float dt) {
	if (FMath::Abs(textRender->XScale - targetScale) <= KINDA_SMALL_NUMBER) return true;

	float value = FMath::FInterpTo(textRender->XScale, targetScale, dt, 8.f);
	textRender->SetXScale(value);
	textRender->SetYScale(value);
	return false;
}

// Called every frame
void ATextDispenser::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	switch (state) {
		case EDispenserState::EDS_dormant: break;
		case EDispenserState::EDS_showing: showingTick(DeltaTime); break;
		case EDispenserState::EDS_hiding: hidingTick(DeltaTime); break;
		case EDispenserState::EDS_active: break;
	}
}

