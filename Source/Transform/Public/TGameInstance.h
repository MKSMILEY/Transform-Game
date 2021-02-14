// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TRANSFORM_API UTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTGameInstance();

protected:

	int LevelIterator;

	FString CurLevel;

	UPROPERTY(EditAnywhere, Category = "Levels")
		TArray<FString> Levels;

	UFUNCTION(BlueprintCallable)
		FString CurLevelName();

	UFUNCTION(BlueprintCallable)
		void SetLevel(int IterMod);

	UFUNCTION(BlueprintCallable)
		void ResetGame();
	
};
