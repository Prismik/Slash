// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigatingWidget.h"

void UNavigatingWidget::loadCompleted() {
	UE_LOG(LogTemp, Display, TEXT("NavigatingWidget::loadCompleted()"));
	if (loadingScreen) {
		loadingScreen->RemoveFromParent();
		loadingScreen = nullptr;
	}
}
