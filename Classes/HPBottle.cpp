#include "HPBottle.h"
#include "ui/UILoadingBar.h"
USING_NS_CC;
using namespace ui;
#include "DataManager.h"
#include "JsonTool.h"

HPBottle* HPBottle::create()
{
	auto hb = new HPBottle();
	if (hb && hb->init())
	{
		hb->autorelease();
		return hb;
	}
	CC_SAFE_DELETE(hb);
	return nullptr;
}

bool HPBottle::init()
{
	if (!Node::create())
	{
		return false;
	}
	this->setCascadeOpacityEnabled(true);
	m_maxnum = m_num = JsonTool::getInstance()->getDoc()["hpbottlemax"].GetInt();
	m_cdTime = 5.0f;
	m_stateName = "01";

	initBG();
	initCD();
	return true;
}

void HPBottle::initBG()
{
	std::string fileStr = "gamescene/ui/hpbottle_bg_";
	m_bg = Sprite::create(StringUtils::format("%s%s_normal.png", fileStr.c_str(), m_stateName.c_str()));
	m_bg->setTag(1);
	this->addChild(m_bg);
	m_label = Label::createWithBMFont("fonts/ui_mid.fnt", StringUtils::format("%d/%d", m_num,m_maxnum));
	m_label->setTag(2);
	this->addChild(m_label);


	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [this, fileStr](Touch* t, Event* e)->bool{
		auto pos = m_bg->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, m_bg->getContentSize() };
		if (rc.containsPoint(pos) && (m_isCD == false))
		{
			m_bg->setTexture(StringUtils::format("%s%s_selected.png", fileStr.c_str(), m_stateName.c_str()));
			return true;
		}
		return false;
	};
	lis->onTouchEnded = [this, fileStr](Touch* t, Event* e)->void{
		auto pos = m_bg->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, m_bg->getContentSize() };
		if (rc.containsPoint(pos))
		{
			if (m_num == 0)
			{
				m_stateName = "02";
				auto gold = DataManager::getInstance()->getGoldNum();
				if (gold < 200)
				{
					m_bg->setTexture(StringUtils::format("%s%s_normal.png", fileStr.c_str(), m_stateName.c_str()));
					return;
				}
			}
			else
			{
				m_num--;
				m_stateName = (m_num == 0) ? ("02") : ("01");
				
			}
			auto cd = static_cast<LoadingBar*>(m_cd->getChildByTag(1));
			m_isCD = true;
			cd->setPercent(100.0f);
			this->scheduleUpdate();
			m_callback();
		}
		if (m_num > 0)
		{
			m_label->setString(StringUtils::format("%d/%d", m_num, m_maxnum));
			m_label->setVisible(true);
		}
		else
		{
			m_label->setVisible(false);
		}
		m_bg->setTexture(StringUtils::format("%s%s_normal.png", fileStr.c_str(), m_stateName.c_str()));
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_bg);
}

void HPBottle::initCD()
{
	m_isCD = false;
	m_cd = Node::create();
	auto cd = LoadingBar::create("gamescene/ui/hpbottle_cd.png", 0);
	cd->setDirection(LoadingBar::Direction::LEFT);
	cd->setTag(1);
	m_cd->addChild(cd);
	
	this->addChild(m_cd);
}

void HPBottle::update(float dt)
{
	static float count = 0;
	count += dt;
	auto percent = 100.0f - count / m_cdTime * 100.0f;
	auto cd = static_cast<LoadingBar*>(m_cd->getChildByTag(1));
	if (percent <= 0)
	{
		cd->setPercent(0);
		m_isCD = false;
		this->unscheduleUpdate();
		count = 0;
	}
	cd->setPercent(percent);
}
