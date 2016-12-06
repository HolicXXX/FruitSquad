#include "HPBottle.h"
#include "ui/UILoadingBar.h"
USING_NS_CC;
using namespace ui;

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
	m_num = 3;
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
				//if(gold < 200)
				//m_bg->setTexture(StringUtils::format("%s%s_normal.png", fileStr.c_str(), m_stateName.c_str()));
				//return;
			}
			else
			{
				m_num--;
				m_stateName = (m_num == 0) ? ("02") : ("01");
				
			}
			//m_num--;
			//if (m_num <= 0)
			//{
			//	m_num = 0;
			//	m_stateName = "02";
			//	//if(gold < 200)
			//	//m_bg->setTexture(StringUtils::format("%s%s_normal.png", fileStr.c_str(), m_stateName.c_str()));
			//	//return;
			//}
			//auto act = ProgressFromTo::create(m_cdTime, 100.0f, 0.0f);
			//m_isCD = true;
			//auto cd = static_cast<ProgressTimer*>(m_cd->getChildByName("pt"));
			//cd->runAction(Sequence::create(act,
			//	CallFunc::create([this]()->void{
			//	m_isCD = false;
			//}), nullptr));
			auto cd = static_cast<LoadingBar*>(m_cd->getChildByTag(1));
			m_isCD = true;
			cd->setPercent(100.0f);
			this->scheduleUpdate();
			m_callback();
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
	
	/*auto cd = ProgressTimer::create(Sprite::create("gamescene/ui/hpbottle_cd.png"));
	cd->setName("pt");
	cd->setType(ProgressTimer::Type::RADIAL);
	cd->setPercentage(0);
	cd->setTag(1);
	m_cd->addChild(cd);*/
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
