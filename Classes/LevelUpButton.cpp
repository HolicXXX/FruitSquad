#include "LevelUpButton.h"
#include "DataManager.h"

LevelUpButton* LevelUpButton::create(Level l, int cost)
{
	auto lub = new LevelUpButton();
	if (lub && lub->init(l,cost))
	{
		lub->autorelease();
		return lub;
	}
	CC_SAFE_DELETE(lub);
	return nullptr;
}

bool LevelUpButton::init(Level l, int cost)
{
	if (!Node::init())
	{
		return false;
	}
	m_button = nullptr;
	m_label = nullptr;
	resetButton(l, cost);

	NotificationCenter::getInstance()->addObserver(this, SEL_CallFuncO(&LevelUpButton::observerCallBack), "resetLevelUpButton", nullptr);
	initTouchListener();
	return true;
}

void LevelUpButton::observerCallBack(Ref*)
{
	resetButton(m_level, m_goldCost);
}

void LevelUpButton::initTouchListener()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* t, Event* e)->bool{
		auto pos = this->m_button->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, this->m_button->getContentSize() };
		if (rc.containsPoint(pos))
		{
			if (this->m_isEnough)
			{
				m_button->setTexture(StringUtils::format("gamescene/ui/levelup_%02d_selected.png", int(m_level) + 1));
			}
			return true;
		}
		return false;
	};
	listener->onTouchEnded = [this](Touch* t, Event* e)->void{
		auto pos = this->m_button->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, this->m_button->getContentSize() };
		if (rc.containsPoint(pos))
		{
			if (this->m_isEnough)
			{
				m_level = Level(int(m_level) + 1);
				m_goldCost *= 2;
				DataManager::getInstance()->costGold(this->m_goldCost / 2);
				this->m_callback();
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void LevelUpButton::resetButton(Level l, int cost)
{
	auto gold = DataManager::getInstance()->getGoldNum();
	m_level = l;
	m_goldCost = cost;
	if (!m_button)
	{
		if (gold >= cost && l != HIGH)
		{
			m_isEnough = true;
			m_button = Sprite::create(StringUtils::format("gamescene/ui/levelup_%02d_normal.png", int(m_level) + 1));
			this->addChild(m_button);
		}
		else
		{
			m_isEnough = false;
			m_button = Sprite::create(StringUtils::format("gamescene/ui/levelup_%02d_disable.png", int(m_level) + 1));
			this->addChild(m_button);
		}
		if (l != Level::HIGH)
		{
			m_label = Label::createWithTTF(StringUtils::format("%d", m_goldCost), "fonts/arial.ttf", 12);
			m_label->setPosition(m_button->getContentSize().width / 2, 17);
			m_button->addChild(m_label);
		}
		return;
	}
	switch (l)
	{
	case LOW:
	{
		if (gold >= m_goldCost)
		{
			m_isEnough = true;
			m_button->setTexture("gamescene/ui/levelup_01_normal.png");
		}
		else
		{
			m_isEnough = false;
			m_button->setTexture("gamescene/ui/levelup_01_disable.png");
		}
		m_label->setString(StringUtils::format("%d", m_goldCost));
	}
		break;
	case MID:
	{
		if (gold >= m_goldCost)
		{
			m_isEnough = true;
			m_button->setTexture("gamescene/ui/levelup_02_normal.png");
		}
		else
		{
			m_isEnough = false;
			m_button->setTexture("gamescene/ui/levelup_02_disable.png");
		}
		m_label->setString(StringUtils::format("%d", m_goldCost));
	}
		break;
	case HIGH:
	{
		m_isEnough = false;
		m_button->setTexture("gamescene/ui/levelup_03_disable.png");
		if (m_label)
		{
			m_label->removeFromParent();
			m_label = nullptr;
		}
	}
		break;
	default:
		break;
	}
}
