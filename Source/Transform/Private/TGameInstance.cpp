// Fill out your copyright notice in the Description page of Project Settings.


#include "TGameInstance.h"

UTGameInstance::UTGameInstance()
{
	// String of level orders in the game and initializing the iterator and current level values.
	Levels = { "L1_FirstJump", "L2_FirstSpeed", "L3_FirstGlide", "L4_FirstCombo" };
	LevelIterator = 0;
	CurLevel = Levels[LevelIterator];
}

// Called by DestructionZoneActor to restart current level.
FString UTGameInstance::CurLevelName()
{
	return CurLevel;
}

// Called whenever a level is changed.
void UTGameInstance::SetLevel(int IterMod)
{
	LevelIterator += IterMod;
	CurLevel = Levels[LevelIterator];
}

// Called whenever the player returns to the title screen.
void UTGameInstance::ResetGame()
{
	LevelIterator = 0;
	CurLevel = Levels[LevelIterator];
}
