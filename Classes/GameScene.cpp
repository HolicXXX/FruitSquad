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

	//scale
	{
		auto mapsize = Size(m_map->getMapSize().width * m_map->getTileSize().width, m_map->getMapSize().height * m_map->getTileSize().height);
		auto scale = ((mapsize.width / size.width) < (mapsize.height / size.height)) ? (mapsize.width / size.width) : (mapsize.height / size.height);
		m_map->setScale(1 / scale);
	}

	initSelectMidLayer();
	initHeroSelect();
	m_heroSelectLayer->bindStartCallBack(CC_CALLBACK_0(GameScene::startCallBack, this));
	return true;
}

void GameScene::initMap()
{
	auto size = Director::getInstance()->getVisibleSize();
	//data
	m_map = TMXTiledMap::create("gamescene/map/level_01.tmx");
	auto decoration = Sprite::create("gamescene/map/level_01.png");
	decoration->setAnchorPoint(Vec2::ZERO);
	m_map->addChild(decoration);
	this->addChild(m_map);

	/*auto obj = m_map->getObjectGroup("object");
	auto start = obj->getObject("start");
	auto offset = Vec2(size / 2) - Vec2(start["x"].asFloat(), start["y"].asFloat());
	if (offset.x >= 0)
	{
		offset.x = 0;
	}
	if (offset.y >= 0)
	{
		offset.y = 0;
	}
	m_map->setPosition(offset);*/
	//TODO:add
	auto obj = m_map->getObjectGroup("object");
	auto mapsize = Size(m_map->getMapSize().width * m_map->getTileSize().width, m_map->getMapSize().height * m_map->getTileSize().height);
	auto start = obj->getObject("start");
	//portal
	m_portal = Node::create();
	m_portal->setPosition(Vec2(start["x"].asFloat(), start["y"].asFloat()));
	{
		auto base = Sprite::create("gamescene/map/start_portal_base.png");
		m_portal->addChild(base);
		auto bg = Sprite::create("gamescene/map/start_portal_bg.png");
		m_portal->addChild(bg);
		auto mid = Sprite::create("gamescene/map/start_portal_mid.png");
		mid->runAction(RepeatForever::create(RotateBy::create(5.0f, 360)));
		m_portal->addChild(mid);
		auto top = Sprite::create("gamescene/map/start_portal_top.png");
		top->runAction(RepeatForever::create(RotateBy::create(3.0f, -360)));
		m_portal->addChild(top);
	}
	m_map->addChild(m_portal, m_map->getLayer("road")->getZOrder());//ZOrder road
}

void GameScene::initSelectMidLayer()
{
	auto size = Director::getInstance()->getVisibleSize();
	m_midLayer = Layer::create();
	this->addChild(m_midLayer);
	auto returnButton = Sprite::create("gamescene/button_return_normal.png");
	returnButton->setPosition(returnButton->getContentSize().width, size.height - returnButton->getContentSize().height);
	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [this, returnButton](Touch* t, Event* e)->bool{
		auto pos = returnButton->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, returnButton->getContentSize() };
		if (rc.containsPoint(pos))
		{
			returnButton->setTexture("gamescene/button_return_selected.png");
			return true;
		}
		return false;
	};
	lis->onTouchMoved = [this, returnButton](Touch* t, Event* e)->void{
		auto pos = returnButton->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, returnButton->getContentSize() };
		if (rc.containsPoint(pos))
		{
			returnButton->setTexture("gamescene/button_return_selected.png");
		}
		else
		{
			returnButton->setTexture("gamescene/button_return_normal.png");
		}
	};
	lis->onTouchEnded = [this, size, returnButton](Touch* t, Event* e)->void{
		auto pos = returnButton->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, returnButton->getContentSize() };
		if (rc.containsPoint(pos))
		{
			returnButton->setTexture("gamescene/button_return_normal.png");
			//callback
			m_heroSelectLayer->runAction(MoveBy::create(0.25f, Vec2(0, -720)));
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, returnButton);
	m_midLayer->addChild(returnButton);

	auto swallowlis = EventListenerTouchOneByOne::create();
	swallowlis->setSwallowTouches(true);
	swallowlis->onTouchBegan = [this, size](Touch* t, Event* e)->bool{
		auto pos = m_midLayer->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, size };
		if (rc.containsPoint(pos))
		{
			return true;
		}
		return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallowlis, m_midLayer);
}

void GameScene::initHeroSelect()
{
	m_heroSelectLayer = HeroSelect::create();
	this->addChild(m_heroSelectLayer);
}

void GameScene::startCallBack()
{
	m_heroSelectLayer->removeFromParent();
	m_midLayer->removeFromParent();
	//start
}

void GameScene::update(float dt)
{

}