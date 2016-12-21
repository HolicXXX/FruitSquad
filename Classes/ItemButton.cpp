#include "ItemButton.h"
#include "2d/CCProgressTimer.h"
#include "JsonTool.h"
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
	//data
	m_number = JsonTool::getInstance()->getDoc()["items"][int(m_type)]["num"].GetInt();
	if (m_number > 0)
		m_state = ButtonState::NORMAL;
	else
		m_state = ButtonState::DISABEL;
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
		if (rc.containsPoint(pos) && (m_state == ButtonState::NORMAL))
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
				if (m_number > 0)
				{
					m_state = CD;
					resetTexture();
					m_number--;
					JsonTool::getInstance()->getDoc()["items"][int(m_type)]["num"].SetInt(m_number);
					resetNubmerLabel();
					auto act = ProgressFromTo::create(m_cdTime, 100.0f, 0.0f);
					auto cd = static_cast<ProgressTimer*>(m_cd->getChildByName("pt"));
					cd->runAction(Sequence::create(act,
						CallFunc::create([this]()->void{
						if (m_number > 0)
						{
							m_state = ButtonState::NORMAL;
						}
						else
						{
							m_state = ButtonState::DISABEL;
						}
						resetTexture();
					}), nullptr));
					m_useCallback();
				}
			}
			else
			{
				setSelected(!m_isSelected);
				NotificationCenter::getInstance()->postNotification("selectItem", this);
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
	m_cdTime = 20.0f;
	m_cd = Node::create();
	m_cd->setTag(2);
	auto cd = ProgressTimer::create(Sprite::create(StringUtils::format("%scd.png", fileStr.c_str())));
	cd->setName("pt");
	cd->setType(ProgressTimer::Type::RADIAL);
	cd->setReverseProgress(true);
	cd->setPercentage(0);
	m_cd->addChild(cd);
	this->addChild(m_cd);
}

void ItemButton::initNumberImage()
{
	std::string fileStr = "gamescene/item/item_";
	m_numberImage = Sprite::create(StringUtils::format("%snum_bg.png", fileStr.c_str()));
	m_numberImage->setPosition(m_item->getContentSize().width / 2 - m_numberImage->getContentSize().width / 2, -(m_item->getContentSize().height / 2 - m_numberImage->getContentSize().height / 2));
	this->addChild(m_numberImage);
	m_numberLabel = Label::createWithTTF(StringUtils::format("%d",m_number), "fonts/arial.ttf", 18);
	m_numberLabel->setTextColor(Color4B::WHITE);
	m_numberLabel->setPosition(m_numberImage->getContentSize() / 2);
	m_numberImage->addChild(m_numberLabel);
}

void ItemButton::resetNubmerLabel()
{
	m_numberLabel->setString(StringUtils::format("%d", m_number));
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
	case I_BOOM:
	{
		m_name = "boom";
	}
		break;
	case I_ANGEL:
	{
		m_name = "angel";
	}
		break;
	case I_FROZEN:
	{
		m_name = "frozen";
	}
		break;
	case I_BUFF:
	{
		m_name = "buff";
	}
		break;
	case I_DRAGON:
	{
		m_name = "dragon";
	}
		break;
	default:
		break;
	}
}

void ItemButton::update(float dt)
{

}