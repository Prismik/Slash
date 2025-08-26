// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactor.generated.h"

class UItemTooltip;
class AInteractable;
class AMainCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UInteractor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractor();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void add(AInteractable* interactable);
	void remove(AInteractable* interactable);
	void cycle();
	void interact();
	AMainCharacter* character;
	UItemTooltip* tooltip;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleInstanceOnly, meta = (AllowPrivateAccess = true))
	TArray<AInteractable*> interactables;

	UPROPERTY(VisibleInstanceOnly, meta = (AllowPrivateAccess = true))
	int32 index = 0;
};
