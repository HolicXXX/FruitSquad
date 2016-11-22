#include "StartScene.h"
#include "LoadingScene.h"
#include "LevelSelectedScene.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace cocostudio;

Scene* StartScene::createScene()
{
	auto start = StartScene::create();
	auto scene = Scene::create();
	scene->addChild(start);
	return scene;
}

StartScene* StartScene::create()
{
	auto start = new StartScene();
	if (start && start->init())
	{
		start->autorelease();
		return start;
	}
	CC_SAFE_DELETE(start);
	return nullptr;
}

bool StartScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto size = Director::getInstance()->getVisibleSize();
	//init sky
	initBG();
	//logo
	auto title = Node::create();
	{
		auto sun = Sprite::create("startscene/sun.png");
		sun->setPosition(0, sun->getContentSize().height / 2);
		title->addChild(sun);
		auto logo = Sprite::create("startscene/logo.png");
		logo->setPosition(0, logo->getContentSize().height / 2);
		auto r1 = RotateBy::create(0.05f, 10);
		auto rr1 = r1->reverse();
		auto r2 = RotateBy::create(0.05f, -10);
		auto rr2 = r2->reverse();
		logo->runAction(RepeatForever::create(Sequence::create(DelayTime::create(5.0f), r1, rr1, r2, rr2, r1, rr1, r2, rr2, nullptr)));
		title->addChild(logo);
	}
	title->setPosition(size.width / 2, size.height / 4 * 3);
	this->addChild(title);
	//rotated ball
	m_ball = Sprite::create("startscene/global_ball.png");
	m_ball->setPosition(size.width / 2, 0);
	m_ball->setRotation(-60);
	m_ball->runAction(RepeatForever::create(RotateBy::create(24.0f, 360)));
	this->addChild(m_ball);
	//hero armature
	ArmatureDataManager::getInstance()->addArmatureFileInfo("startscene/start0.png", "startscene/start0.plist", "startscene/start.ExportJson");
	auto hero = Armature::create("start");
	hero->setPosition(size.width / 5 * 2, size.height / 2);
	hero->getAnimation()->play("hero");
	this->addChild(hero);
	//car armature
	auto car = Armature::create("start");
	car->setPosition(size.width / 5 * 3, size.height / 2);
	car->getAnimation()->play("car");
	this->addChild(car);
	//rotated trees
	auto trees = Sprite::create("startscene/global_trees.png");
	trees->setPosition(size.width / 2, 0);
	trees->setRotation(-60);
	trees->runAction(RepeatForever::create(RotateBy::create(24.0f, 360)));
	this->addChild(trees);
	//touch to start label
	auto touchToStart = Sprite::create("startscene/touch_start.png");
	touchToStart->setPosition(size.width / 2, size.height / 6);
	touchToStart->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.75f), FadeIn::create(0.75f), nullptr)));
	this->addChild(touchToStart);
	//bind repalce scene listener
	auto lis = EventListenerTouchOneByOne::create();
	lis->onTouchBegan = [](Touch* t, Event* e)->bool{
		return true;
	};
	lis->onTouchEnded = [](Touch* t, Event* e)->void{
		auto scene = Scene::create();
		auto l = LoadingScene::create();
		l->bindNextSceneCallBack(LevelSelectedScene::createScene);
		scene->addChild(l);
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, this);

	this->scheduleUpdate();
	return true;
}

void StartScene::update(float dt)
{
	//

}

void StartScene::initBG()
{
	for (size_t i = 0; i < 7; i++)
	{
		auto sky = Sprite::create("startscene/skyblue_bg.png");
		sky->setPosition(Vec2(i * sky->getContentSize().width, 0) + sky->getContentSize() / 2);
		this->addChild(sky);
		auto seq = Sequence::create(DelayTime::create(7), FadeOut::create(1), DelayTime::create(15), FadeIn::create(1),nullptr);
		sky->runAction(RepeatForever::create(seq));
		m_skyblue.pushBack(sky);
	}
	for (size_t i = 0; i < 7; i++)
	{
		auto sky = Sprite::create("startscene/yellow_bg.png");
		sky->setPosition(Vec2(i * sky->getContentSize().width, 0) + sky->getContentSize() / 2);
		this->addChild(sky);
		auto seq = Sequence::create(DelayTime::create(7), FadeIn::create(1), DelayTime::create(7), FadeOut::create(1),DelayTime::create(8), nullptr);
		sky->runAction(RepeatForever::create(seq));
		sky->setOpacity(0);
		m_yellow.pushBack(sky);
	}
	for (size_t i = 0; i < 7; i++)
	{
		auto sky = Sprite::create("startscene/blue_bg.png");
		sky->setPosition(Vec2(i * sky->getContentSize().width, 0) + sky->getContentSize() / 2);
		this->addChild(sky);
		auto seq = Sequence::create(DelayTime::create(15), FadeIn::create(1), DelayTime::create(7), FadeOut::create(1), nullptr);
		sky->runAction(RepeatForever::create(seq));
		sky->setOpacity(0);
		m_yellow.pushBack(sky);
	}

}