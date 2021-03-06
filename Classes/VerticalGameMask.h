// ---------------------------------------------------------------------------------------------------
// -------------------------------- SquareTapper by Maciej Pryc, 2017 --------------------------------
// ---------------------------------------------------------------------------------------------------

#pragma once

#include "SquareTapper.h"
#include "GameMask.h"

class VerticalGameMask : public GameMask
{
protected:
	int CurrentColumnIndex;

// ---------------------------------------------------------------------------------------------------
public:
	VerticalGameMask(GameScene* argScene, bool bKillingMask);

protected:
	virtual void UpdateSpritePosition();
	virtual void Move();
	virtual void FrozeSquareActivation();
};
