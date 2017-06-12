// ---------------------------------------------------------------------------------------------------
// -------------------------------- SquareTapper by Maciej Pryc, 2017 --------------------------------
// ---------------------------------------------------------------------------------------------------

#pragma once

#define COCOS2D_DEBUG 1

#include "cocos2d.h"

enum class ESquareState { Inactive, DuringActivation, Completed, Failed };

struct LevelParams
{
	int LevelDisplayNumber;
	int WorldNumber;
	int LevelNumber;
	int DangerousSquaresNumber;
	float SquaresActivationTimeInterval;
	float TotalSquareActivationTime;
	std::vector<int> DoubleTapSquareIndices;
	std::vector<int> SequenceSquareIndices;
	bool bSpawnGameMask;
	bool bVerticalMask;
	bool bKillingMask;

	LevelParams():
	LevelDisplayNumber(-1),
	WorldNumber(-1),
	LevelNumber(-1),
	DangerousSquaresNumber(-1),
	SquaresActivationTimeInterval(-1.0f), 
	TotalSquareActivationTime(-1.0f),
	bSpawnGameMask(false),
	bVerticalMask(false),
	bKillingMask(false)
	{};
};

std::string GetLevelKey(const int LevelDisplayName);
