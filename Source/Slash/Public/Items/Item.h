// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

enum class EItemState : uint8 {
	EIS_hovering,
	EIS_equipped
};

UCLASS()
class SLASH_API AItem : public AInteractable
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Sin parameters")
	float amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Sin parameters")
	float timeConstant = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Rotation parameters")
	float rotationSpeed = 20.f;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category="Item|Sin parameters")
	float computeSinTransformation();

	UFUNCTION(BlueprintPure, Category="Item|Sin parameters")
	float computeCosTransformation();

	template<typename T>
	T avg(T lhs, T rhs);

	virtual void sphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void sphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	EItemState state = EItemState::EIS_hovering;
	
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Item")
	float runTime;
};

template <typename T>
T AItem::avg(T lhs, T rhs) {
	return (lhs + rhs) / 2;
}
