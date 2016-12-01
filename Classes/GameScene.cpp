#include "GameScene.h"
#include "orange.h"

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

	//TODO:add
	auto obj = m_map->getObjectGroup("object");
	auto mapsize = Size(m_map->getMapSize().width * m_map->getTileSize().width, m_map->getMapSize().height * m_map->getTileSize().height);
	auto start = obj->getObject("start");
	//portal
	m_portal = Node::create();
	m_portal->setPosition(Vec2(start["x"].asFloat(), start["y"].asFloat()));
	{
		auto base = Sprite::create("gamescene/map/start_portal_base.png");
		base->setTag(1);
		m_portal->addChild(base);
		auto bg = Sprite::create("gamescene/map/start_portal_bg.png");
		bg->setTag(2);
		m_portal->addChild(bg);
		auto mid = Sprite::create("gamescene/map/start_portal_mid.png");
		mid->setTag(3);
		mid->runAction(RepeatForever::create(RotateBy::create(5.0f, 360)));
		m_portal->addChild(mid);
		auto top = Sprite::create("gamescene/map/start_portal_top.png");
		top->setTag(4);
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
	//render
	auto rt = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
	rt->clear(0, 0, 0, 0.66f);
	rt->setPosition(size / 2);
	m_midLayer->addChild(rt);

	auto returnButton = Sprite::create("gamescene/button_return_normal.png");
	returnButton->setTag(1);
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
	auto size = Director::getInstance()->getVisibleSize();
	m_map->setScale(1.0f);
	auto obj = m_map->getObjectGroup("object");
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
	m_map->setPosition(offset);

	m_heroSelectLayer->removeFromParent();
	m_midLayer->removeChildByTag(1);
	//add ui layer
	initUI();
	//start
	m_startEff = Node::create();
	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/start/start_count_down.ExportJson");
	auto eff = Armature::create("start_count_down");
	eff->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(GameScene::aniCallBack, this));
	eff->getAnimation()->play("action1");
	m_startEff->addChild(eff);
	m_startEff->setPosition(size / 2);
	m_midLayer->addChild(m_startEff);
}

void GameScene::aniCallBack(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	if (movementType == MovementEventType::LOOP_COMPLETE)
	{
		if (movementID.compare("action1") == 0)
		{
			armature->getAnimation()->play("action2");
		}
		else if (movementID.compare("action2") == 0)
		{
			armature->getAnimation()->play("action3");
		}
		else if (movementID.compare("action3") == 0)
		{
			m_midLayer->removeFromParent();
			//start
			CCLOG("start callback");
			initHeros();
			this->scheduleUpdate();
		}
	}
}

void GameScene::initHeros()
{
	auto ora = orange::create();
	ora->setPosition(m_portal->getPosition());
	auto dir = Vec2(1, 0);//data
	if (dir.x > 0)
		ora->setRight(true);
	ora->setDir(dir);
	ora->setBaseState(HeroState::WALK);
	ora->setVisi(false);
	m_map->addChild(ora, m_map->getLayer("road")->getZOrder());
	m_heros.pushBack(ora);
}

void GameScene::initUI()
{
	//
}

void GameScene::update(float dt)
{
	if (m_heros.at(0)->isVisi() == false)
	{
		for (auto h : m_heros)
		{
			auto portalSize = static_cast<Sprite*>(m_portal->getChildByTag(2))->getContentSize();
			auto rc = Rect{ -Vec2(portalSize / 2), portalSize };
			auto pos = m_portal->convertToNodeSpace(h->convertToWorldSpace(Vec2::ZERO));
			if (!rc.containsPoint(pos))
			{
				h->setVisi(true);
			}
		}
	}
}