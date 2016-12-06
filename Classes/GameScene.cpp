#include "GameScene.h"
#include "orange.h"
#include "AnimationManager.h"
#include "GameMenuLayer.h"
#include "LoadingScene.h"
#include "LevelSelectedScene.h"
#include "DataManager.h"

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
	AnimationManager::getInstance()->loadGameSceneAni();

	//init map
	initMap();

	//scale
	{
		auto scale = ((m_mapSize.width / size.width) < (m_mapSize.height / size.height)) ? (m_mapSize.width / size.width) : (m_mapSize.height / size.height);
		m_map->setScale(1 / scale);
		m_previewScale = 1 / scale;
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

	//map drag
	auto lis = EventListenerTouchOneByOne::create();
	lis->onTouchBegan = [this](Touch* t, Event* e)->bool{
		return true;
	};
	lis->onTouchMoved = [this,size](Touch* t, Event* e)->void{
		auto nextPos = m_map->getPosition() + t->getDelta();
		if (nextPos.x >= 0)
		{
			nextPos.x = 0;
		}
		else if (nextPos.x <= size.width - m_mapSize.width * m_previewScale)
		{
			nextPos.x = size.width - m_mapSize.width * m_previewScale;
		}
		if (nextPos.y >= 0)
		{
			nextPos.y = 0;
		}
		else if (nextPos.y <= size.height - m_mapSize.height * m_previewScale)
		{
			nextPos.y = size.height - m_mapSize.height * m_previewScale;
		}
		this->m_map->setPosition(nextPos);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_map);

	auto obj = m_map->getObjectGroup("sign");
	auto mapsize = Size(m_map->getMapSize().width * m_map->getTileSize().width, m_map->getMapSize().height * m_map->getTileSize().height);
	m_mapSize = mapsize;
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

}

void GameScene::initHeroSelect()
{
	m_heroSelectLayer = HeroSelect::create();
	this->addChild(m_heroSelectLayer);
}

void GameScene::startCallBack()
{
	auto size = Director::getInstance()->getVisibleSize();
	m_previewScale = 1.0f;
	m_map->setScale(1.0f);
	auto obj = m_map->getObjectGroup("sign");
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

	//add ui
	initUI();
	//render
	auto rt = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
	rt->setName("rt");
	rt->clear(0, 0, 0, 0.5f);
	rt->setPosition(size / 2);
	m_midLayer->addChild(rt);
	auto swallowlis = EventListenerTouchOneByOne::create();
	swallowlis->setSwallowTouches(true);
	swallowlis->onTouchBegan = [this, size](Touch* t, Event* e)->bool{
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallowlis, rt);
	
	//start
	m_startEff = Node::create();
	auto eff = Armature::create("start_count_down");
	eff->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(GameScene::aniCallBack, this));
	eff->getAnimation()->play("action1");
	m_startEff->addChild(eff);
	m_startEff->setPosition(size / 2);
	m_midLayer->addChild(m_startEff);
}

ValueMap GameScene::changeCheckPoint(ValueMap cp)
{
	auto ret = cp;
	if (cp["name"].asString().compare("end") != 0)
	{
		auto obj = m_map->getObjectGroup("sign");
		auto vec = obj->getObjects();
		auto next = cp["tp"].asInt() + 1;
		int i;
		for (i = 0; i < vec.size(); i++)
		{
			auto p = vec.at(i).asValueMap();
			if (p["tp"].asInt() == next)
			{
				break;
			}
		}
		if (i != vec.size())
		{
			auto p = vec.at(i).asValueMap();
			ret = p;
		}
		else
		{
			auto end = obj->getObject("end");
			ret = end;
		}
	}
	return ret;
}

void GameScene::aniCallBack(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	if (movementType == MovementEventType::LOOP_COMPLETE)
	{
		if (movementID.compare("action1") == 0)
		{
			initHeros();
			armature->getAnimation()->play("action2");
		}
		else if (movementID.compare("action2") == 0)
		{
			initSkills();
			armature->getAnimation()->play("action3");
		}
		else if (movementID.compare("action3") == 0)
		{
			m_midLayer->removeChildByName("rt");
			m_midLayer->removeChild(m_startEff);
			m_hp->setOpacity(255);
			m_gold->setOpacity(255);

			for (auto h : m_heros)
			{
				auto dir = h->getNextDir();
				h->setDir(dir);
			}
			//first walk others wait
			m_heros.at(0)->setBaseState(HeroState::WALK);
			m_heros.at(0)->scheduleUpdate();
			for (auto s : m_skills)
			{
				s->setOpacity(255);
			}
			this->scheduleUpdate();
		}
	}
}

void GameScene::initHeros()
{
	auto list = DataManager::getInstance()->getTeamList();
	auto count = DataManager::getInstance()->getTeamNumber();
	for (int i = 0; i < count; ++i)
	{
		auto h = getHero(list[i]);
		h->setPosition(m_portal->getPosition());
		m_map->addChild(h, m_map->getLayer("road")->getZOrder());
		m_heros.pushBack(h);
	}
	/*auto ora = orange::create();
	ora->setPosition(m_portal->getPosition());
	ora->setVisi(false);

	auto obj = m_map->getObjectGroup("sign");
	auto start = obj->getObject("start");
	ora->setCheckPoint(start);
	ora->bindCheckCallBack(CC_CALLBACK_1(GameScene::changeCheckPoint, this));
	
	m_map->addChild(ora, m_map->getLayer("road")->getZOrder());
	m_heros.pushBack(ora);*/
}

HeroBase* GameScene::getHero(int index)
{
	HeroBase* h = nullptr;
	switch (index)
	{
	case 0:
	{
		h = orange::create();
		h->setVisi(false);
		auto obj = m_map->getObjectGroup("sign");
		auto start = obj->getObject("start");
		h->setCheckPoint(start);
		h->bindCheckCallBack(CC_CALLBACK_1(GameScene::changeCheckPoint, this));
	}
	break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	default:
		break;
	}
	return h;
}

void GameScene::initSkills()
{
	auto size = Director::getInstance()->getVisibleSize();
	auto ori = Vec2(size.width, 0);
	auto list = DataManager::getInstance()->getTeamList();
	auto count = DataManager::getInstance()->getTeamNumber();
	for (int i = 0; i < count; ++i)
	{
		auto s = SkillButton::create(list[i]);
		s->bindSkillCallBack(CC_CALLBACK_0(HeroBase::useSkill, m_heros.at(i)));
		s->setPosition(ori + Vec2(-100, (i + 1) * 100));
		s->setOpacity(0);
		m_midLayer->addChild(s);
		m_skills.pushBack(s);
	}
}

void GameScene::pauseAll()
{
	for (auto h : m_heros)
	{
		h->pauseAll();
	}
	//enemys
	this->unscheduleUpdate();
}

void GameScene::resumeAll()
{
	for (auto h : m_heros)
	{
		h->resumeAll();
	}
	//enemys
	this->scheduleUpdate();
}

void GameScene::initUI()
{
	auto size = Director::getInstance()->getVisibleSize();
	auto lefttop = Vec2(0, size.height);
	//pause
	m_pause = UIButton::create(ButtonType::PAUSE);
	m_pause->setPosition(lefttop + Vec2(68, -68));
	m_pause->bindCallBack([this]()->void{
		pauseAll();
	});
	m_pause->bindSecCallBack([this]()->void{
		resumeAll();
	});
	m_midLayer->addChild(m_pause);
	//speed
	m_speed = UIButton::create(ButtonType::SPEED_1);
	m_speed->setPosition(lefttop + Vec2(68, -68 * 2.5f));
	m_speed->bindCallBack([]()->void{
		Director::getInstance()->setAnimationInterval(1.0f / 40.0f);
	});
	m_speed->bindSecCallBack([]()->void{
		Director::getInstance()->setAnimationInterval(1.0f / 20.0f);
	});
	m_midLayer->addChild(m_speed);
	//menu
	m_menu = UIButton::create(ButtonType::MENU);
	m_menu->setPosition(lefttop + Vec2(68 * 2.5f, -68));
	m_menu->bindCallBack([this]()->void{
		this->pauseAll();
		this->addMenuLayer();
	});
	m_midLayer->addChild(m_menu);

	auto righttop = Vec2(size.width, size.height);
	//gold
	m_gold = GoldLabel::create();
	m_gold->setPosition(righttop - Vec2(100, 50));
	m_gold->setOpacity(0);
	m_midLayer->addChild(m_gold);
	//hp
	m_hp = HPBottle::create();
	m_hp->setPosition(righttop - Vec2(100, 130));
	m_hp->bindCallBack([this]()->void{
		CCLOG("use hpbottle");
	});
	m_hp->setOpacity(0);
	m_midLayer->addChild(m_hp);
}

void GameScene::addMenuLayer()
{
	auto menuLayer = GameMenuLayer::create();
	menuLayer->bindContinueCallBack(CC_CALLBACK_0(GameScene::resumeAll, this));
	menuLayer->bindRetryCallBack([]()->void{
		auto scene = Scene::create();
		auto load = LoadingScene::create();
		load->bindNextSceneCallBack(GameScene::createScene);
		scene->addChild(load);
		Director::getInstance()->replaceScene(TransitionMoveInL::create(0.5f, scene));
	});
	menuLayer->bindReturnCallBack([]()->void{
		auto scene = Scene::create();
		auto load = LoadingScene::create();
		load->bindNextSceneCallBack(LevelSelectedScene::createScene);
		scene->addChild(load);
		Director::getInstance()->replaceScene(TransitionMoveInL::create(0.5f, scene));
	});
	this->addChild(menuLayer);
}

void GameScene::update(float dt)
{
	//at begin
	if (m_heros.at(m_heros.size() - 1)->isVisi() == false)
	{
		for (int i = 0; i < m_heros.size();++i)
		{
			auto h = m_heros.at(i);
			auto portalSize = static_cast<Sprite*>(m_portal->getChildByTag(2))->getContentSize();
			auto rc = Rect{ -Vec2(portalSize / 2), portalSize };
			auto pos = m_portal->convertToNodeSpace(h->convertToWorldSpace(Vec2::ZERO));
			if (!rc.containsPoint(pos))
			{
				h->setVisi(true);
				if (i + 1 != m_heros.size())
				{
					m_heros.at(i + 1)->setBaseState(HeroState::WALK);
					m_heros.at(i + 1)->scheduleUpdate();
				}
			}
		}
	}
	//each
	for (auto h : m_heros)
	{
		h->pointCheck();
	}
}