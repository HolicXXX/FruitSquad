#include "SettlementLayer.h"
#include "HeroHallLayer.h"
USING_NS_CC;
#include "DataManager.h"
#include "JsonTool.h"

SettlementLayer* SettlementLayer::create(int deathtimes)
{
	auto sl = new SettlementLayer();
	if (sl && sl->init(deathtimes))
	{
		sl->autorelease();
		return sl;
	}
	CC_SAFE_DELETE(sl);
	return nullptr;
}

bool SettlementLayer::init(int deathtimes)
{
	if (!Layer::init())
	{
		return false;
	}
	this->setCascadeOpacityEnabled(true);
	m_deathTimes = deathtimes;
	m_getStarNum = m_getFruitNum = 0;
	initBG();
	initStar();
	initGoalBar();
	NotificationCenter::getInstance()->addObserver(this, SEL_CallFuncO(&SettlementLayer::settle), "settle", nullptr);
	initFruits();
	initButton();

	initHeroHallButton();
	return true;
}

void SettlementLayer::settle(Ref*)
{
	getExtraStarNum();
	int extra = getExtraFruitNum();
	if (extra > 0)
		JsonTool::getInstance()->getDoc()["fruit"].SetInt(extra + JsonTool::getInstance()->getDoc()["fruit"].GetInt());
	JsonTool::getInstance()->saveJson();
}

void SettlementLayer::onExit()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
	Node::onExit();
}

void SettlementLayer::initHeroHallButton()
{
	m_herohallButton = Node::create();
	auto button = Sprite::create("gamescene/settlement/button_upgrade_normal.png");
	button->setName("button");
	button->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_herohallButton->addChild(button);
	auto label = Sprite::create("gamescene/settlement/button_upgrade_label.png");
	label->setPosition(45, -45);
	label->setName("label");
	m_herohallButton->addChild(label);
	auto arrow = Sprite::create("gamescene/settlement/button_upgrade_arrow.png");
	arrow->setPosition(Vec2(95, 45));
	arrow->setName("arrow");
	arrow->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.3f, Vec2(0, 20)), MoveBy::create(0.3f, Vec2(0, -20)), nullptr)));
	m_herohallButton->addChild(arrow);
	m_herohallButton->setPosition(468, 100);
	m_bg->addChild(m_herohallButton);

	auto layer = HeroHall::create();
	this->addChild(layer);
	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [this, button](Touch* t, Event* e)->bool{
		auto pos = button->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, button->getContentSize() };
		if (rc.containsPoint(pos))
		{
			button->setTexture("gamescene/settlement/button_upgrade_selected.png");
			return true;
		}
		return false;
	};
	lis->onTouchEnded = [this, button, layer](Touch* t, Event* e)->void{
		auto pos = button->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, button->getContentSize() };
		auto size = Director::getInstance()->getVisibleSize();
		if (rc.containsPoint(pos))
		{
			layer->runAction(MoveTo::create(0.3f, Vec2(0, 0)));
		}
		button->setTexture("gamescene/settlement/button_upgrade_normal.png");
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, button);
}

void SettlementLayer::initStar()
{
	m_stars = SettlementStar::create();
	m_stars->setPosition(m_bg->getContentSize().width / 2, m_bg->getContentSize().height);
	m_bg->addChild(m_stars);
}

void SettlementLayer::initGoalBar()
{
	float per = 0;
	if (m_deathTimes == 0)
		per = 1.0f;
	else if (m_deathTimes >= 7)
	{
		per = 1.0f / 9.0f;
	}
	else
	{
		per = 1.0f / 9.0f * (8 - m_deathTimes);
	}
	m_goal = ScalableBar::create(per);
	m_goal->setPosition(85, 368);
	m_goal->bindCallBack1(CC_CALLBACK_0(SettlementLayer::goalCallBack1, this));
	m_goal->bindCallBack2(CC_CALLBACK_0(SettlementLayer::goalCallBack2, this));
	m_goal->bindCallBack3(CC_CALLBACK_0(SettlementLayer::goalCallBack3, this));
	m_goal->scheduleUpdate();
	m_bg->addChild(m_goal);
}

void SettlementLayer::initFruits()
{
	auto level = DataManager::getInstance()->getMapLevel();
	auto orifruit = JsonTool::getInstance()->getDoc()["springlevels"][level - 1]["fruit"].GetInt();
	//data
	auto fruit1 = SettlementFruit::create(false, 2);
	fruit1->setPosition(Vec2(115,231));
	m_bg->addChild(fruit1);
	m_fruits.pushBack(fruit1);

	auto fruit2 = SettlementFruit::create(false, 1);
	fruit2->setPosition(Vec2(234,231));
	m_bg->addChild(fruit2);
	m_fruits.pushBack(fruit2);

	auto fruit3 = SettlementFruit::create(false, 2);
	fruit3->setPosition(Vec2(352,231));
	m_bg->addChild(fruit3);
	m_fruits.pushBack(fruit3);
	if (orifruit >= 2)
		fruit1->getFruit();
	if (orifruit >= 3)
		fruit2->getFruit();
	if (orifruit == 5)
		fruit3->getFruit();
}

void SettlementLayer::goalCallBack1()
{
	m_stars->addStar();
	m_fruits.at(0)->getFruit();
	++m_getStarNum;
	m_getFruitNum += 2;
}

void SettlementLayer::goalCallBack2()
{
	m_stars->addStar();
	m_fruits.at(1)->getFruit();
	++m_getStarNum;
	m_getFruitNum += 1;
}

void SettlementLayer::goalCallBack3()
{
	m_stars->addStar();
	m_fruits.at(2)->getFruit();
	++m_getStarNum;
	m_getFruitNum += 2;
}

int SettlementLayer::getExtraFruitNum()
{
	auto level = DataManager::getInstance()->getMapLevel();
	auto orifruit = JsonTool::getInstance()->getDoc()["springlevels"][level - 1]["fruit"].GetInt();
	int extra = m_getFruitNum - orifruit;
	if (extra > 0)
	{
		JsonTool::getInstance()->getDoc()["springlevels"][level - 1]["fruit"].SetInt(m_getFruitNum);
		return extra;
	}
	else
		return 0;
}

int SettlementLayer::getExtraStarNum()
{
	auto level = DataManager::getInstance()->getMapLevel();
	auto oristar = JsonTool::getInstance()->getDoc()["springlevels"][level - 1]["star"].GetInt();
	int extra = m_getStarNum - oristar;
	if (extra > 0)
	{
		JsonTool::getInstance()->getDoc()["springlevels"][level - 1]["star"].SetInt(m_getStarNum);
		return extra;
	}
	else
		return 0;
}

void SettlementLayer::initBG()
{
	auto size = Director::getInstance()->getVisibleSize();

	auto rt = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
	rt->setPosition(size / 2);
	rt->clear(0, 0, 0, 0.75f);
	rt->setName("rt");
	this->addChild(rt);
	auto swallow = EventListenerTouchOneByOne::create();
	swallow->setSwallowTouches(true);
	swallow->onTouchBegan = [this,rt,size](Touch* t, Event* e)->bool{
		auto pos = rt->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, size };
		if (rc.containsPoint(pos))
		{
			return true;
		}
		return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallow, rt);

	m_eff = Sprite::create("gamescene/settlement/settlement_eff.png");
	m_eff->setPosition(size / 2);
	m_eff->runAction(RepeatForever::create(RotateBy::create(1.0f, 90)));
	this->addChild(m_eff);
	m_bg = Sprite::create("gamescene/settlement/settlement_bg.png");
	m_bg->setPosition(size / 2);
	this->addChild(m_bg);
	m_title = Sprite::create("gamescene/settlement/title_spring.png");
	m_title->setPosition(168, 486);
	m_bg->addChild(m_title);
	//add level label
	m_key = Sprite::create("gamescene/settlement/get_key.png");
	m_key->setPosition(77, 136);
	m_bg->addChild(m_key);
	//add num label and gems
	m_keyLabel = Label::createWithTTF("0", "fonts/arial.ttf", 24);
	m_keyLabel->setPosition(145, 136);
	m_bg->addChild(m_keyLabel);
	auto gemNum = 5 - m_deathTimes;
	if (gemNum <= 0)
		gemNum = 0;
	m_gemLabel = Label::createWithTTF(StringUtils::format("%d", gemNum), "fonts/arial.ttf", 24);
	m_gemLabel->setPosition(340, 136);
	m_bg->addChild(m_gemLabel);
	auto level = DataManager::getInstance()->getMapLevel();
	auto origem = JsonTool::getInstance()->getDoc()["gems"].GetInt();
	JsonTool::getInstance()->getDoc()["gems"].SetInt(origem + gemNum);
}

void SettlementLayer::initButton()
{
	m_retry = UIButton::create(ButtonType::MENU_RETRY);
	m_retry->setPosition(Vec2(143, 30));
	m_bg->addChild(m_retry);

	m_next = UIButton::create(ButtonType::NEXT);
	m_next->setPosition(Vec2(333, 30));
	m_bg->addChild(m_next);
}

void SettlementLayer::bindRetryCallBack(const std::function<void()>& func)
{
	m_retry->bindCallBack(func);
}

void SettlementLayer::bindNextCallBack(const std::function<void()>& func)
{
	m_next->bindCallBack(func);
}