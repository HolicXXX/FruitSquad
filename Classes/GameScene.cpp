#include "GameScene.h"
#include "orange.h"
#include "coconut.h"
#include "apple.h"
#include "scarab.h"
#include "GameMenuLayer.h"
#include "LoadingScene.h"
#include "LevelSelectedScene.h"

#include "AnimationManager.h"
#include "DataManager.h"
#include "TargetManger.h"
#include "MapManager.h"
#include "BoxCullisionManager.h"
#include "JsonTool.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

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

	if (JsonTool::getInstance()->getDoc()["sound"].GetBool())
		if (JsonTool::getInstance()->getDoc()["music"].GetBool())
			if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
				SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	auto level = DataManager::getInstance()->getMapLevel();
	auto goldNum = JsonTool::getInstance()->getDoc()["springlevels"][level - 1]["gold"].GetInt();
	DataManager::getInstance()->setGoldNum(goldNum);

	m_deathCount = 0;
	NotificationCenter::getInstance()->addObserver(this, SEL_CallFuncO(&GameScene::addDeathCount), "addDeathCount", nullptr);
	//init map
	initMap();
	initEnemys();
	//scale
	{
		auto msize = MapManager::getInstance()->getMapSize();
		auto scale = ((msize.width / size.width) < (msize.height / size.height)) ? (msize.width / size.width) : (msize.height / size.height);
		m_map->setScale(1 / scale);
		m_previewScale = 1 / scale;
	}

	initSelectMidLayer();
	initHeroSelect();
	m_heroSelectLayer->bindStartCallBack(CC_CALLBACK_0(GameScene::startCallBack, this));
	
	return true;
}

void GameScene::addDeathCount(Ref*)
{
	++m_deathCount;
}

void GameScene::onExit()
{
	auto level = DataManager::getInstance()->getMapLevel();
	JsonTool::getInstance()->saveJson();
	AnimationManager::getInstance()->eraseGameSceneAni(level);
	Node::onExit();
	NotificationCenter::getInstance()->removeAllObservers(this);
}

void GameScene::initMap()
{
	auto size = Director::getInstance()->getVisibleSize();
	auto level = DataManager::getInstance()->getMapLevel();
	auto mapsrc = JsonTool::getInstance()->getDoc()["springlevels"][level - 1]["map"].GetString();
	m_map = TMXTiledMap::create(mapsrc);
	MapManager::getInstance()->registMap(m_map);
	auto decsrc = JsonTool::getInstance()->getDoc()["springlevels"][level - 1]["decoration"].GetString();
	auto decoration = Sprite::create(decsrc);
	decoration->setAnchorPoint(Vec2::ZERO);
	m_map->addChild(decoration, m_map->getLayer("road")->getZOrder());
	this->addChild(m_map);
	auto msize = MapManager::getInstance()->getMapSize();
	//map drag
	auto lis = EventListenerTouchOneByOne::create();
	lis->onTouchBegan = [this](Touch* t, Event* e)->bool{
		return true;
	};
	lis->onTouchMoved = [this,size,msize](Touch* t, Event* e)->void{
		auto nextPos = m_map->getPosition() + t->getDelta();
		if (nextPos.x >= 0)
		{
			nextPos.x = 0;
		}
		else if (nextPos.x <= size.width - msize.width * m_previewScale)
		{
			nextPos.x = size.width - msize.width * m_previewScale;
		}
		if (nextPos.y >= 0)
		{
			nextPos.y = 0;
		}
		else if (nextPos.y <= size.height - msize.height * m_previewScale)
		{
			nextPos.y = size.height - msize.height * m_previewScale;
		}
		this->m_map->setPosition(nextPos);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_map);

	auto obj = m_map->getObjectGroup("sign");
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

void GameScene::cameraMove(Vec2 step)
{
	auto size = Director::getInstance()->getVisibleSize();
	auto firstPos = this->convertToNodeSpace(m_heros.at(0)->convertToWorldSpace(Vec2::ZERO));
	if (Rect{ Vec2::ZERO, size }.containsPoint(firstPos))
	{
		auto mapsize = MapManager::getInstance()->getMapSize();
		auto newPos = m_map->getPosition() - step;
		if (newPos.x >= 0)
			newPos.x = 0;
		if (newPos.x <= size.width - mapsize.width)
			newPos.x = size.width - mapsize.width;
		if (newPos.y >= 0)
			newPos.y = 0;
		if (newPos.y <= size.height - mapsize.height)
			newPos.y = size.height - mapsize.height;
		m_map->setPosition(newPos);
	}
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
	//reset map scale and position
	m_previewScale = 1.0f;
	m_map->setScale(m_previewScale);
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
	if (m_map->getPositionY() <= size.height - MapManager::getInstance()->getMapSize().height)
	{
		m_map->setPositionY(size.height - MapManager::getInstance()->getMapSize().height);
	}
	if (m_map->getPositionX() <= size.width - MapManager::getInstance()->getMapSize().width)
	{
		m_map->setPositionX(size.height - MapManager::getInstance()->getMapSize().height);
	}

	m_midLayer->removeChildByTag(1);
	//add ui
	initUI();
	initItems();
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

	m_heroSelectLayer->removeFromParent();
}

void GameScene::initItems()
{
	auto item = m_heroSelectLayer->getSelectedItem();
	if (item == nullptr)
		return;
	m_item = ItemButton::create(ButtonRole::USE, item->getItemType());
	m_item->setPosition(100, 100);
	m_item->bindUseItemCallBack([this]()->void{
		useItem(m_item->getItemType());
	});
	m_midLayer->addChild(m_item);
}

void GameScene::useItem(ItemType type)
{
	auto size = Director::getInstance()->getVisibleSize();
	switch (type)
	{
	case I_BOOM:
		break;
	case I_ANGEL:
		break;
	case I_FROZEN:
	{
		auto arm = Armature::create("ice");
		if (JsonTool::getInstance()->getDoc()["sound"].GetBool())
			SimpleAudioEngine::getInstance()->playEffect("sound/item_freeze.wav");
		arm->getAnimation()->play("ice");
		arm->getAnimation()->setMovementEventCallFunc(
			[this,size](Armature *armature, MovementEventType movementType, const std::string& movementID)->void{
			if (movementType == LOOP_COMPLETE)
			{
				auto bg = Sprite::create("gamescene/ani/eff/ice/debuff_freeze_bg.png");
				bg->setName("ice_bg");
				bg->setPosition(size / 2);
				this->addChild(bg);
				bg->runAction(Sequence::create(DelayTime::create(5.0f),
					CallFunc::create([bg]()->void{
					bg->removeFromParent();
				}),
					nullptr));
				auto enemyVec = TargetManager::getInstance()->getEnemyTargets();
				for (auto e : enemyVec)
				{
					auto freeze = DeBuff::create();
					freeze->type = DeBuffType::FREEZE;
					freeze->time = 5.0f;
					freeze->demage = 0;
					freeze->eff = Armature::create("freeze");
					freeze->eff->getAnimation()->play("freeze");
					freeze->eff->getAnimation()->setMovementEventCallFunc(
						[freeze](Armature *armature, MovementEventType movementType, const std::string& movementID)->void{
						if (movementType == LOOP_COMPLETE)
						{
							if (movementID == "freeze")
								armature->getAnimation()->play("stay");
							else if (movementID == "melt")
								freeze->removeFromParent();
						}
					});
					freeze->effName = "freeze";
					freeze->addChild(freeze->eff);
					freeze->setPosition(Vec2(0, -30));
					e->getDeBuff(freeze);
				}
				armature->removeFromParent();
			}
		});
		arm->setPosition(size / 2);
		this->addChild(arm);
	}
		break;
	case I_BUFF:
		break;
	case I_DRAGON:
		break;
	default:
		break;
	}
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
		if (i == 0)
			h->bindCameraMoveCallBack(CC_CALLBACK_1(GameScene::cameraMove, this));
	}
	TargetManager::getInstance()->registHero(m_heros);
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
	{
		h = coconut::create();
		h->setVisi(false);
		auto obj = m_map->getObjectGroup("sign");
		auto start = obj->getObject("start");
		h->setCheckPoint(start);
		h->bindCheckCallBack(CC_CALLBACK_1(GameScene::changeCheckPoint, this));
	}
	break;
	case 2:
	{
		h = apple::create();
		h->setVisi(false);
		auto obj = m_map->getObjectGroup("sign");
		auto start = obj->getObject("start");
		h->setCheckPoint(start);
		h->bindCheckCallBack(CC_CALLBACK_1(GameScene::changeCheckPoint, this));
	}
	break;
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

void GameScene::initEnemys()
{
	auto l = m_map->getObjectGroup("enemy");
	auto vec = l->getObjects();
	for (int i = 0; i < vec.size(); i++)
	{
		auto obj = vec[i].asValueMap();
		auto pos = Vec2(obj["x"].asFloat(), obj["y"].asFloat());
		auto e = scarab::create();
		e->setPosition(pos);
		e->setLevel(Level(int(CCRANDOM_0_1() * 3)));
		m_map->addChild(e, m_map->getLayer("road")->getZOrder());
		m_enemys.pushBack(e);
	}
	TargetManager::getInstance()->registEnemy(m_enemys);
}

void GameScene::pauseAll()
{
	for (auto h : m_heros)
	{
		if (h->isAlive())
			h->pauseAll();
	}
	for (auto e : m_enemys)
	{
		if (e->isAlive())
			e->pauseAll();
	}
	//buttons
	this->unscheduleUpdate();
}

void GameScene::resumeAll()
{
	for (auto h : m_heros)
	{
		if (h->isAlive())
			h->resumeAll();
	}
	for (auto e : m_enemys)
	{
		if (e->isAlive())
			e->resumeAll();
	}
	//buttons
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
	auto fps = Director::getInstance()->getAnimationInterval();
	auto speed = (fps > 1.0f / 30.0f) ? (ButtonType::SPEED_1) : (ButtonType::SPEED_2);
	m_speed = UIButton::create(speed);
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
	NotificationCenter::getInstance()->addObserver(this, SEL_CallFuncO(&GameScene::getGold), "addGoldAni", nullptr);
	//hp
	m_hp = HPBottle::create();
	m_hp->setPosition(righttop - Vec2(100, 130));
	m_hp->bindCallBack([this]()->void{
		auto hvec = TargetManager::getInstance()->getHeroTargets();
		for (auto h : hvec)
		{
			if (h->isAlive())
			{
				auto buff = Buff::create();
				buff->eff = Armature::create("heal");
				buff->eff->getAnimation()->play("heal");
				buff->addChild(buff->eff);
				buff->type = BuffType::HEAL;
				buff->effName = "heal";
				buff->time = 4.0f;
				buff->buffNum = 50.0f;
				h->getBuff(buff);
			}
		}
	});
	m_hp->setOpacity(0);
	m_midLayer->addChild(m_hp);
}

void GameScene::getGold(Ref* ref)
{
	auto e = static_cast<EnemyBase*>(ref);
	if (!e->isAlive())
	{
		auto pos = m_midLayer->convertToNodeSpace(e->convertToWorldSpace(Vec2::ZERO));
		auto gold = Armature::create("gold");
		gold->setPosition(pos);
		gold->getAnimation()->play("gold");
		gold->setScale(0.3f);
		gold->runAction(
			Sequence::create(MoveTo::create(0.75f, m_gold->getPosition()),
			CallFunc::create([this, e, gold]()->void{
			auto get = Armature::create("get");
			get->getAnimation()->play("get");
			get->setPosition(m_gold->getPosition());
			m_midLayer->addChild(get);
			get->getAnimation()->setMovementEventCallFunc(
				[this, gold](Armature *armature, MovementEventType movementType, const std::string& movementID)->void{
				if (movementType == LOOP_COMPLETE)
				{
					armature->removeFromParent();
				}
			});
			m_gold->getCoin(e->getGoldNum());
			gold->removeFromParent();
		}),
			nullptr));
		m_midLayer->addChild(gold);
	}
}

void GameScene::addMenuLayer()
{
	auto menuLayer = GameMenuLayer::create();
	menuLayer->bindContinueCallBack(CC_CALLBACK_0(GameScene::resumeAll, this));
	menuLayer->bindRetryCallBack([]()->void{
		auto scene = Scene::create();
		auto load = LoadingScene::create();
		load->bindNextSceneCallBack(GameScene::createScene);
		load->setNextSceneAni(NextSceneType::GAME_SCENE, 20);
		scene->addChild(load);
		Director::getInstance()->replaceScene(TransitionMoveInL::create(0.5f, scene));
	});
	menuLayer->bindReturnCallBack([]()->void{
		auto scene = Scene::create();
		auto load = LoadingScene::create();
		load->bindNextSceneCallBack(LevelSelectedScene::createScene);
		load->setNextSceneAni(NextSceneType::POINT_LELECT_SCENE, 2);
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
	//move check
	for (auto h : m_heros)
	{
		h->pointCheck();
	}
	//zorder
	for (int i = 0; i < m_heros.size(); ++i)
	{
		auto h = m_heros.at(i);
		h->setZOrder(MapManager::getInstance()->getMapSize().height - h->getPositionY());
	}
	//skill hit check
	auto hbvec = BoxCullisionManager::getInstance()->m_heroBox;
	for (auto hb : hbvec)
	{
		BoxCullisionManager::getInstance()->hitEnemy(hb);
	}

	bool isend = true;
	for (auto h : m_heros)
	{
		isend = isend && (h->getBaseState() == HeroState::STOP);
	}
	if (isend)
	{
		this->pauseAll();
		this->runAction(Sequence::create(DelayTime::create(1.0f) ,CallFunc::create([this](){
			callSettleMent();
		}),nullptr));
	}
}

void GameScene::callSettleMent()
{
	auto level = DataManager::getInstance()->getMapLevel();
	JsonTool::getInstance()->getDoc()["springlevels"][level - 1]["death"].SetInt(m_deathCount);
	m_settleMent = SettlementLayer::create(0);
	m_settleMent->bindRetryCallBack([]()->void{
		auto scene = Scene::create();
		auto load = LoadingScene::create();
		load->bindNextSceneCallBack(GameScene::createScene);
		load->setNextSceneAni(NextSceneType::GAME_SCENE, 20);
		scene->addChild(load);
		Director::getInstance()->replaceScene(TransitionMoveInL::create(0.5f, scene));
	});
	m_settleMent->bindNextCallBack([]()->void{
		auto scene = Scene::create();
		auto load = LoadingScene::create();
		load->bindNextSceneCallBack(LevelSelectedScene::createScene);
		load->setNextSceneAni(NextSceneType::POINT_LELECT_SCENE, 2);
		scene->addChild(load);
		Director::getInstance()->replaceScene(TransitionMoveInL::create(0.5f, scene));
	});
	this->addChild(m_settleMent);
	if (JsonTool::getInstance()->getDoc()["sound"].GetBool())
		SimpleAudioEngine::getInstance()->playEffect("sound/settlement.wav");
	JsonTool::getInstance()->getDoc()["springlevels"][level - 1]["state"].SetInt(2);
	if (JsonTool::getInstance()->getDoc()["springlevels"][level]["state"].GetInt() == 0)
		JsonTool::getInstance()->getDoc()["springlevels"][level]["state"].SetInt(1);
}