// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hittable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHittable : public UInterface {
	GENERATED_BODY()
};

class SLASH_API IHittable {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void hit(const FVector& p);
};
