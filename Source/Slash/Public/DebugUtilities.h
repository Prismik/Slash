// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameFramework/Actor.h"
#include "DebugUtilities.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API UDebugUtilities : public UObject {
	GENERATED_BODY()

public:
	static void print(const FString& str);
	static void drawCoordinates(UWorld* world, AActor* actor, bool once = false);
	static void drawArrow(UWorld* world, const FVector& origin, const FVector& end, bool once = false, FColor color = FColor::Emerald);
	static void drawSphere(UWorld* world, const FVector& p, float  radius = 32.f, bool once = false, FColor color = FColor::Emerald);
	static void drawPoint(UWorld* world, AActor* actor, bool once = false, FColor color = FColor::Red);
	static void drawPoint(UWorld* world, FVector p, bool once = false, FColor color = FColor::Red);
};
