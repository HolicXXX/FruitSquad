#include "ItemButton.h"
#include "2d/CCProgressTimer.h"
USING_NS_CC;

ItemButton* ItemButton::create(ButtonRole r, ItemType t)
{
	auto ib = new ItemButton();
	if (ib && ib->init(r, t))
	{
		ib->autorelease();
		return ib;
	}
	CC_SAFE_DELETE(ib);
	return nullptr;
}

bool ItemButton::init(ButtonRole r, ItemType t)
{
	if (!Node::init())
	{
		return false;
	}
	this->setCascadeOpacityEnabled(true);
	m_role = r;
	m_type = t;
	m_state = ItemButtonState::NORMAL;
	initName();

	initItem();
	initEff();
	initCD();
	initNumberImage();

	return true;
}

void ItemButton::initItem()
{
	std::string fileStr = "gamescene/item/item_";
	std::string stateStr = (m_number == 0) ? ("disable") : ("normal");
	m_item = Sprite::create(StringUtils::format("%s%s_%s.png", fileStr.c_str(), m_name.c_str(), stateStr.c_str()));
	m_item->setTag(1);
	this->addChild(m_item);
	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [this](Touch* t, Event* e)->bool{
		auto pos = m_item->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, m_item->getContentSize() };
		if (rc.containsPoint(pos) && (m_state == ItemButtonState::NORMAL))
		{
			m_item->setScale(0.99f);
			return true;
		}
		return false;
	};
	lis->onTouchEnded = [this](Touch* t, Event* e)->void{
		auto pos = m_item->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, m_item->getContentSize() };
		if (rc.containsPoint(pos))
		{
			if (m_role == USE)
			{
				m_state = CD;
				resetTexture();
				auto act = ProgressFromTo::create(m_cdTime, 100.0f, 0.0f);
				auto cd = static_cast<ProgressTimer*>(m_cd->getChildByName("pt"));
				cd->runAction(Sequence::create(act,
					CallFunc::create([this]()->void{
					m_state = ItemButtonState::NORMAL;
					resetTexture();
				}), nullptr));
				m_useCallback();
			}
			else
			{
				m_isSelected = !m_isSelected;
				m_eff->setVisible(m_isSelected);
			}
		}
		m_item->setScale(1.0f);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_item);
}

void ItemButton::initEff()
{
	std::string fileStr = "gamescene/item/item_";
	m_eff = Sprite::create(StringUtils::format("%seff_selected.png", fileStr.c_str()));
	m_eff->setTag(2);
	m_eff->setVisible(false);
	m_isSelected = false;
	this->addChild(m_eff);
}

void ItemButton::setSelected(bool b)
{
	m_isSelected = b;
	m_eff->setVisible(b);
}

void ItemButton::initCD()
{
	std::string fileStr = "gamescene/skill/skill_";
	m_cd = Node::create();
	m_cd->setTag(2);
	auto cd = ProgressTimer::create(Sprite::create(StringUtils::format("%scd.png", fileStr.c_str())));
	cd->setName("pt");
	cd->setType(ProgressTimer::Type::RADIAL);
	cd->setPercentage(0);
	m_cd->addChild(cd);
	this->addChild(m_cd);
}

void ItemButton::initNumberImage()
{
	//data
	m_number = 5;
	std::string fileStr = "gamescene/skill/skill_";
	m_numberImage = Sprite::create(StringUtils::format("%snum_bg.png", fileStr.c_str()));
	m_numberImage->setPosition(m_item->getContentSize() / 2);
	this->addChild(m_numberImage);
}

void ItemButton::resetTexture()
{
	std::string fileStr = "gamescene/item/item_";
	switch (m_state)
	{
	case NORMAL:
	{
		m_item->setTexture(StringUtils::format("%s%s_normal.png", fileStr.c_str(), m_name.c_str()));
	}
		break;
	case CD:
	{
		m_item->setTexture(StringUtils::format("%s%s_disable.png", fileStr.c_str(), m_name.c_str()));
	}
		break;
	case DISABEL:
	{
		m_item->setTexture(StringUtils::format("%s%s_disable.png", fileStr.c_str(), m_name.c_str()));
	}
		break;
	default:
		break;
	}
}

void ItemButton::initName()
{
	switch (m_type)
	{
	case BOOM:
	{
		m_name = "boom";
	}
		break;
	case ANGEL:
	{
		m_name = "angel";
	}
		break;
	case FROZEN:
	{
		m_name = "frozen";
	}
		break;
	case BUFF:
	{
		m_name = "buff";
	}
		break;
	case DRAGON:
	{
		m_name = "dragon";
	}
		break;
	default:
		break;
	}
}