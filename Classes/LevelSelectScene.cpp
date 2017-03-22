#include "LevelSelectScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* LevelSelectScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LevelSelectScene::create();

    scene->addChild(layer);

    return scene;
}

bool LevelSelectScene::init()
{
	if (!Layer::init())
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float FontSize = 50.0f / Director::getInstance()->getContentScaleFactor();
	auto label = Label::createWithTTF("Choose level", "fonts/Marker Felt.ttf", FontSize);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.9f));
	this->addChild(label, 1);

	auto Level1Button = MenuItemImage::create("Level1_idle.png", "Level1_pressed.png",
	[&](Ref* sender) {
		Director::getInstance()->replaceScene(HelloWorld::createScene());
	});

	Level1Button->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.7f));
	auto LevelMenu = Menu::create(Level1Button, nullptr);
	LevelMenu->setPosition(Vec2::ZERO);
	this->addChild(LevelMenu, 1);

	return true;
}