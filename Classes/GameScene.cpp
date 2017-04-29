// ---------------------------------------------------------------------------------------------------
// -------------------------------- SquareTapper by Maciej Pryc, 2017 --------------------------------
// ---------------------------------------------------------------------------------------------------

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "GameSquare.h"
#include "LevelSelectScene.h"
#include "VerticalGameMask.h"
#include "HorizontalGameMask.h"

USING_NS_CC;

GameScene::GameScene(int argLevelNumber, float argTimeBetweenSquaresActivation, float argSquareActivationTotalTime, bool argbSpawnGameMask):
Mask(nullptr),
LevelNumber(argLevelNumber),
StartDelay(0.5f),
MaxTimeWithoutActiveSquare(0.5f),
TimeBetweenSquaresActivation(argTimeBetweenSquaresActivation),
SquareActivationTotalTime(argSquareActivationTotalTime),
SquarePositionMarginX(0.23f),
SquarePositionMarginY(0.2f),
UnactivatedSquaresNumber(SQUARE_AMOUNT_X * SQUARE_AMOUNT_Y),
bSpawnGameMask(argbSpawnGameMask)
{

}

GameScene* GameScene::create(int argLevelNumber, float argTimeBetweenSquaresActivation, float argSquareActivationTotalTime, bool argbSpawnGameMask)
{
	GameScene *pRet = new(std::nothrow) GameScene(argLevelNumber, argTimeBetweenSquaresActivation, argSquareActivationTotalTime, argbSpawnGameMask);

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool GameScene::init()
{
    if (!Scene::init())
        return false;
    
    VisibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vector<MenuItem*> MenuItems;

    auto BackItem = MenuItemImage::create("Back_idle.png", "Back_pressed.png",
		[&](Ref* sender) {
		Director::getInstance()->replaceScene(LevelSelectScene::create());
	});
    
	BackItem->setPosition(Vec2(origin.x + VisibleSize.width * 0.77f, origin.y + VisibleSize.height * 0.07f));
	MenuItems.pushBack(BackItem);

	auto RestartItem = MenuItemImage::create("Restart_idle.png", "Restart_pressed.png",
		[&](Ref* sender) {
		Director::getInstance()->replaceScene(GameScene::create(LevelNumber, TimeBetweenSquaresActivation, SquareActivationTotalTime, bSpawnGameMask));
	});

	RestartItem->setPosition(Vec2(origin.x + VisibleSize.width * 0.88f, origin.y + VisibleSize.height * 0.07f));
	MenuItems.pushBack(RestartItem);

    auto menu = Menu::createWithArray(MenuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
	float FontSize = 50.0f / Director::getInstance()->getContentScaleFactor();
    auto label = Label::createWithTTF("Tap the squares!", "fonts/Marker Felt.ttf", FontSize);
	label->setPosition(Vec2(origin.x + VisibleSize.width / 2.0f, origin.y + VisibleSize.height * 0.955f));
	this->addChild(label, 1);

	FontSize = 36.0f / Director::getInstance()->getContentScaleFactor();
	std::stringstream Stream;
	Stream << "Level " << LevelNumber;
	label = Label::createWithTTF(Stream.str(), "fonts/Marker Felt.ttf", FontSize);
	label->setPosition(Vec2(origin.x + VisibleSize.width / 2.0f, origin.y + VisibleSize.height * 0.9f));
	this->addChild(label, 1);

    auto sprite = Sprite::create("HelloWorld.png");
    sprite->setPosition(Vec2(VisibleSize.width/2 + origin.x, VisibleSize.height/2 + origin.y));
	this->addChild(sprite, 0);

	for (int x = 0; x < SQUARE_AMOUNT_X; ++x)
	{
		for (int y = 0; y < SQUARE_AMOUNT_Y; ++y)
		{
			Vec2 Pos = Vec2(GetScreenPositionX(x), GetScreenPositionY(y));
			GameSquare* NewSquare = new GameSquare(this, Pos, x, y);
			Squares[x][y] = NewSquare;
		}
	}

	if (bSpawnGameMask)
		Mask = new HorizontalGameMask(this);

	auto StartDelayAction = DelayTime::create(bSpawnGameMask ? StartDelay + 0.7f : StartDelay);
	auto ActivateFirstSquareAction = CallFunc::create([&]() {ActivateNextSquare(); });
	runAction(Sequence::create(StartDelayAction, ActivateFirstSquareAction, nullptr));

	CCLOG("Initialization end");

    return true;
}

void GameScene::onExit()
{
	Scene::onExit();

	for (int x = 0; x < SQUARE_AMOUNT_X; ++x)
		for (int y = 0; y < SQUARE_AMOUNT_Y; ++y)
			delete Squares[x][y];

	if (Mask)
		delete Mask;
}

float GameScene::GetScreenPositionX(int SquareIndexX) const
{
	return GetScreenPosition(SquareIndexX, SQUARE_AMOUNT_X, SquarePositionMarginX, VisibleSize.width);
}

float GameScene::GetScreenPositionY(int SquareIndexY) const
{
	return GetScreenPosition(SquareIndexY, SQUARE_AMOUNT_Y, SquarePositionMarginY, VisibleSize.height);
}

float GameScene::GetScreenPosition(int SquareIndex, int SquaresNumber, float SquarePositionMargin, float ScreenSize) const
{
	float PosMod = SquarePositionMargin + (float)SquareIndex / (SquaresNumber - 1) * (1.0f - 2.0f * SquarePositionMargin);
	return ScreenSize * PosMod;
}
GameSquare* GameScene::GetSquareForActivation() const
{
	std::vector<GameSquare*> AvailableSquares;

	for (int x = 0; x < SQUARE_AMOUNT_X; ++x)
	{
		for (int y = 0; y < SQUARE_AMOUNT_Y; ++y)
		{
			if (Squares[x][y]->GetState() == ESquareState::Inactive && Squares[x][y]->GetCoveredByMask() == false)
				AvailableSquares.push_back(Squares[x][y]);
		}
	}

	std::random_shuffle(AvailableSquares.begin(), AvailableSquares.end());
	return AvailableSquares.size() > 0 ? AvailableSquares[0] : nullptr;
}

void GameScene::ActivateNextSquare()
{
	if (GameSquare* NextSquare = GetSquareForActivation())
	{
		NextSquare->StartActivation(SquareActivationTotalTime);
		ActiveSquares.push_back(NextSquare);
		--UnactivatedSquaresNumber;
	}

	if (UnactivatedSquaresNumber > 0)
		QueueNextSquareActivation(TimeBetweenSquaresActivation);
}

void GameScene::QueueNextSquareActivation(float Delay)
{
	auto DelayAction = DelayTime::create(Delay);
	auto ActivateNextSquareAction = CallFunc::create([&]() {ActivateNextSquare(); });
	auto MySequence = Sequence::create(DelayAction, ActivateNextSquareAction, nullptr);
	MySequence->setTag(ACTIVATION_SEQUENCE_ACTION_TAG);
	runAction(MySequence);
}

void GameScene::OnSquareCompleted(GameSquare* CompletedSquare)
{
	ActiveSquares.erase(std::remove(ActiveSquares.begin(), ActiveSquares.end(), CompletedSquare));

	if (ActiveSquares.empty())
	{
		auto ActionManager = Director::getInstance()->getActionManager();
		auto ReturnedAction = ActionManager->getActionByTag(ACTIVATION_SEQUENCE_ACTION_TAG, this);

		if (Sequence* ReturnedSequence = dynamic_cast<Sequence*>(ReturnedAction))
		{
			const float SequenceRemainingTime = ReturnedSequence->getDuration() - ReturnedSequence->getElapsed();

			if (SequenceRemainingTime > MaxTimeWithoutActiveSquare)
			{
				ActionManager->removeAllActionsFromTarget(this);
				QueueNextSquareActivation(MaxTimeWithoutActiveSquare);
			}
		}
		else
		{
			auto sprite = Sprite::create("SmallFrame.png");
			sprite->setPosition(Vec2(VisibleSize.width * 0.5f, VisibleSize.height * 0.55f));
			this->addChild(sprite, 4);

			float FontSize = 50.0f / Director::getInstance()->getContentScaleFactor();
			auto label = Label::createWithTTF("You win!", "fonts/Marker Felt.ttf", FontSize);
			label->setPosition(Vec2(VisibleSize.width * 0.5f, VisibleSize.height * 0.55f));
			this->addChild(label, 5);
		}
	}
}

void GameScene::OnSquareFailed(GameSquare* FailedSquare)
{
	ActiveSquares.erase(std::remove(ActiveSquares.begin(), ActiveSquares.end(), FailedSquare));

	for (GameSquare* CurrSquare : ActiveSquares)
		CurrSquare->PauseOnGameOver();
	
	if (Mask)
		Mask->OnGameOver();

	Director::getInstance()->getActionManager()->removeAllActionsFromTarget(this);

	auto sprite = Sprite::create("SmallFrame.png");
	sprite->setPosition(Vec2(VisibleSize.width * 0.5f, VisibleSize.height * 0.55f));
	this->addChild(sprite, 4);

	float FontSize = 50.0f / Director::getInstance()->getContentScaleFactor();
	auto label = Label::createWithTTF("You lost!", "fonts/Marker Felt.ttf", FontSize);
	label->setPosition(Vec2(VisibleSize.width * 0.5f, VisibleSize.height * 0.55f));
	this->addChild(label, 5);
}
