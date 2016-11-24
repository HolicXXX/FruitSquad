#include "GameScene.h"
USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}
GameScene* GameScene::create()
{
	auto gs = new GameScene();
	if (gs && gs->init())
	{
		gs->autorelease();
		return gs;
	}
	CC_SAFE_DELETE(gs);
	return nullptr;
}
bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto size = Director::getInstance()->getVisibleSize();
	//init map
	initMap();
	
	initHeroSelect();
	return true;
}

void GameScene::initMap()
{
	auto size = Director::getInstance()->getVisibleSize();
	//data
	m_map = TMXTiledMap::create("gamescene/map/level_01.tmx");
	//m_map->setScale(0.5f);
	auto decoration = Sprite::create("gamescene/map/level_01.png");
	decoration->setAnchorPoint(Vec2::ZERO);
	m_map->addChild(decoration);
	this->addChild(m_map);
	auto obj = m_map->getObjectGroup("object");
	auto start = obj->getObject("start");
	//
	auto offset = Vec2(size / 2) - Vec2(start["x"].asFloat(), start["y"].asFloat());
	if (offset.x >= 0)
	{
		offset.x = 0;
	}
	if (offset.y >= 0)
	{
		offset.y = 0;
	}
	m_map->setPosition(offset);
	//TODO:add

}

void GameScene::initHeroSelect()
{
	m_heroSelectLayer = HeroSelect::create();
	this->addChild(m_heroSelectLayer);
}

void GameScene::update(float dt)
{

}