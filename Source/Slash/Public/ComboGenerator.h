// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ComboGenerator.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UComboGenerator : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IComboGenerator {
	GENERATED_BODY()

public:
	virtual TArray<UAnimMontage*> getCombos() = 0;
};
