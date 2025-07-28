// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class AMainCharacter;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class SLASH_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractable();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void interact(AMainCharacter* character);
	
	virtual void focus();
	virtual void unfocus();
	
	UFUNCTION()
	virtual void sphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void sphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;
	UMaterialInterface* overlay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	USphereComponent* sphere;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category="Item")
	UStaticMeshComponent* mesh;
	
	bool canInteract = true;
};
