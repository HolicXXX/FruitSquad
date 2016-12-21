#include "SkillButton.h"
#include "2d/CCProgressTimer.h"
#include "DataManager.h"
USING_NS_CC;

SkillButton* SkillButton::create(int index)
{
	auto sb = new SkillButton();
	if (sb && sb->init(index))
	{
		sb->autorelease();
		return sb;
	}
	CC_SAFE_DELETE(sb);
	return nullptr;
}

bool SkillButton::init(int index)
{
	if (!Node::init())
	{
		return false;
	}
	this->setCascadeOpacityEnabled(true);
	m_index = index;
	setCDTime();
	m_name = DataManager::getInstance()->getName(m_index);
	m_isCD = false;
	m_state = NORMAL;
	m_effTime = 0;
	m_effState = NONE;
	m_effCount = 0;
	m_skill = nullptr;
	m_cd = nullptr;
	m_eff = nullptr;

	resetButton();

	return true;
}

void SkillButton::resetButton()
{
	std::string fileStr = "gamescene/skill/skill_";
	if (!m_skill)
	{
		m_skill = Sprite::create(StringUtils::format("%s%s_normal.png", fileStr.c_str(), m_name.c_str()));
		m_skill->setTag(1);
		this->addChild(m_skill);
		//lsitener
		auto lis = EventListenerTouchOneByOne::create();
		lis->setSwallowTouches(true);
		lis->onTouchBegan = [this](Touch* t, Event* e)->bool{
			auto pos = m_skill->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, m_skill->getContentSize() };
			if (rc.containsPoint(pos) && (m_state == ButtonState::NORMAL) && (m_effState == ButtonEffect::NONE))
			{
				m_skill->setScale(0.9f);
				return true;
			}
			return false;
		};
		lis->onTouchEnded = [this](Touch* t, Event* e)->void{
			auto pos = m_skill->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, m_skill->getContentSize() };
			if (rc.containsPoint(pos) && m_callback())
			{
				m_state = CD;
				resetTexture();
			}
			m_skill->setScale(1.0f);
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_skill);
	}
}

void SkillButton::resetTexture()
{
	std::string fileStr = "gamescene/skill/skill_";
	switch (m_state)
	{
	case NORMAL:
	{
		m_skill->setTexture(StringUtils::format("%s%s_normal.png", fileStr.c_str(), m_name.c_str()));
	}
		break;
	case CD:
	{
		m_skill->setTexture(StringUtils::format("%s%s_disable.png", fileStr.c_str(), m_name.c_str()));
		if (!m_cd)
		{
			m_cd = Node::create();
			m_cd->setTag(2);
			auto cd = ProgressTimer::create(Sprite::create(StringUtils::format("%scd.png", fileStr.c_str())));
			cd->setType(ProgressTimer::Type::RADIAL);
			m_cd->addChild(cd);
			cd->setReverseProgress(true);
			auto act = ProgressFromTo::create(m_cdTime, 100.0f, 0.0f);
			cd->runAction(Sequence::create(act, 
				CallFunc::create([this]()->void{
				m_state = ButtonState::NORMAL;
				resetTexture();
				m_cd->removeFromParent();
				m_cd = nullptr;
			}), nullptr));
			this->addChild(m_cd);
		}
	}
		break;
	case DISABEL:
	{
		m_skill->setTexture(StringUtils::format("%s%s_disable.png", fileStr.c_str(), m_name.c_str()));
	}
		break;
	default:
		break;
	}
}

void SkillButton::setEffect(ButtonEffect eff, float t)
{
	std::string fileStr = "gamescene/skill/skill_eff_";
	switch (eff)
	{
	case NONE:
	{
		if (m_eff)
		{
			m_eff->removeFromParent();
			m_eff = nullptr;
			this->unscheduleUpdate();
		}
	}
		break;
	case FROZEN:
	{
		if (m_eff)
		{
			if (m_effState != FROZEN)
			{
				m_eff->removeFromParent();
				m_eff = Sprite::create(StringUtils::format("%sfrozen.png", fileStr.c_str()));
				m_eff->setTag(3);
				this->addChild(m_eff);
				this->scheduleUpdate();
			}
		}
		else
		{
			m_eff = Sprite::create(StringUtils::format("%sfrozen.png", fileStr.c_str()));
			m_eff->setTag(3);
			this->addChild(m_eff);
			this->scheduleUpdate();
		}
	}
		break;
	case SLEEP:
	{
		if (m_eff)
		{
			if (m_effState != SLEEP)
			{
				m_eff->removeFromParent();
				m_eff = Sprite::create(StringUtils::format("%ssleep.png", fileStr.c_str()));
				m_eff->setTag(3);
				this->addChild(m_eff);
				this->scheduleUpdate();
			}
		}
		else
		{
			m_eff = Sprite::create(StringUtils::format("%ssleep.png", fileStr.c_str()));
			m_eff->setTag(3);
			this->addChild(m_eff);
			this->scheduleUpdate();
		}
	}
		break;
	case FORBIDDEN:
	{
		if (m_eff)
		{
			if (m_effState != FORBIDDEN)
			{
				m_eff->removeFromParent();
				m_eff = Sprite::create(StringUtils::format("%sforbidden.png", fileStr.c_str()));
				m_eff->setTag(3);
				this->addChild(m_eff);
				this->scheduleUpdate();
			}
		}
		else
		{
			m_eff = Sprite::create(StringUtils::format("%sforbidden.png", fileStr.c_str()));
			m_eff->setTag(3);
			this->addChild(m_eff);
			this->scheduleUpdate();
		}
	}
		break;
	default:
		break;
	}
	m_effTime = t;
	m_effState = eff;
}

void SkillButton::setCDTime()
{
	switch (m_index)
	{
	case 0:
	{
		m_cdTime = 7.5f;
	}
	break;
	case 1:
	{
		m_cdTime = 8.0f;
	}
	break;
	case 2:
	{
		m_cdTime = 5.0f;
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
}

void SkillButton::update(float dt)
{
	m_effCount += dt;
	if (m_effCount >= m_effTime)
	{
		m_effCount = 0;
		setEffect(ButtonEffect::NONE);
	}
}

