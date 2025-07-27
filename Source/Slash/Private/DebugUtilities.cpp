// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugUtilities.h"

void UDebugUtilities::print(const FString& str) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, str);
	}
}

void UDebugUtilities::drawCoordinates(UWorld* world, AActor* actor, bool once) {
	if (world) {
		const FVector p = actor->GetActorLocation();
		const FVector d = actor->GetActorForwardVector();
		const FVector t = actor->GetActorRightVector();
		const FVector u = actor->GetActorUpVector();
		DrawDebugDirectionalArrow(world, p, 25.f * d + p, 1.f, FColor::Red, once, -1.f, 0, 1.f);
		DrawDebugDirectionalArrow(world, p, 25.f * t + p, 1.f, FColor::Green, once, -1.f, 0, 1.f);
		DrawDebugDirectionalArrow(world, p, 25.f * u + p, 1.f, FColor::Blue, once, -1.f, 0, 1.f);
	}
}

void UDebugUtilities::drawArrow(UWorld* world, const FVector& origin, const FVector& end, bool once, FColor color) {
	DrawDebugDirectionalArrow(world, origin, end, 1.f, color, false, once ? 3.f : -1.f, 0, 1.f);
}

void UDebugUtilities::drawSphere(UWorld* world, const FVector& p, float radius, bool once, FColor color) {
	if (world) {
		DrawDebugSphere(world, p, radius, 24, color, false, once ? 3.f : -1.f);
	}
}

void UDebugUtilities::drawPoint(UWorld* world, AActor* actor, bool once, FColor color) {
	const FVector p = actor->GetActorLocation();
	drawPoint(world, p, once, color);
}

void UDebugUtilities::drawPoint(UWorld* world, FVector p, bool once, FColor color) {
	if (world) {
		DrawDebugPoint(world, p, 10.0, color, once, once ? 30.f : -1.f);
	}
}