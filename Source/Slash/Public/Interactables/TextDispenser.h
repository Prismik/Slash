// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "TextDispenser.generated.h"

class USoundBase;
class UTextRenderComponent;

UENUM(BlueprintType)
enum class EDispenserState : uint8 {
	EDS_dormant,
	EDS_showing,
	EDS_active,
	EDS_hiding
};

UCLASS()
class SLASH_API ATextDispenser : public AInteractable {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "TextDispenser|SFX")
	USoundBase* letterTypeSound;

	UPROPERTY(EditAnywhere, Category = "TextDispenser|SFX")
	USoundBase* appearSound;

	UPROPERTY(EditAnywhere, Category = "TextDispenser|SFX")
	USoundBase* disappearSound;
	
	UPROPERTY(EditAnywhere, Category = "TextDispenser|Display")
	float averageTypingSpeed = 0.3f;
	
	UPROPERTY(EditAnywhere, Category = "TextDispenser|Display")
	bool once = false;

	UPROPERTY(EditAnywhere, Category = "TextDispenser|Display")
	FString text;
	
	ATextDispenser();

	void show();
	void hide();
	
	virtual void Tick(float DeltaTime) override;
	virtual void interact_Implementation(AMainCharacter* character) override;
	virtual void sphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	virtual void BeginPlay() override;
	
private:
	FTimerHandle typingTimer;
	FString currentlyTypedText = FString();
	int32 index = -1;
	float targetScale = 0.f;
	uint8 shownTime = 0;

	UPROPERTY(VisibleAnywhere, Category = "TextDispenser|Display")
	EDispenserState state = EDispenserState::EDS_dormant;

	UPROPERTY(EditDefaultsOnly, Category = "TextDispenser|Display")
	UTextRenderComponent* textRender;

	void typeLetter();
	void showingTick(float dt);
	void hidingTick(float dt);

	float letterTypeDelay();
	bool updateProgress(float dt);
};
