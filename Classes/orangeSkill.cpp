#include "orangeSkill.h"
#include "BoxTool.h"
#include "MapManager.h"
#include "BoxCullisionManager.h"

orangeSkill* orangeSkill::create(Level l, cocos2d::Vec2 dir)
{
	auto os = new orangeSkill();
	if (os && os->init(l, dir))
	{
		os->autorelease();
		return os;
	}
	CC_SAFE_DELETE(os);
	return nullptr;
}

bool orangeSkill::init(Level l, cocos2d::Vec2 dir)
{
	if (!Node::init())
	{
		return false;
	}
	m_level = l;
	m_speed = 5 + 2 * int(l);
	m_dir = dir;
	initEff();
	initAni();
	initDir();
	
	return true;
}

void orangeSkill::initEff()
{
	m_type = DeBuffType::BURN;
	m_effName = "burn";
	m_effDemage = 200 * (int(m_level) + 1);
	m_effTime = 5.0f;
	m_demageTimes = 10;
}

DeBuff* orangeSkill::getDeBuff()
{
	auto debuff = DeBuff::create();
	debuff->type = m_type;
	debuff->effName = m_effName;
	debuff->demage = m_effDemage;
	debuff->time = m_effTime;
	//m_demageTimes;//init everyone

	debuff->eff = Armature::create(m_effName);
	debuff->eff->getAnimation()->play(m_effName);
	debuff->addChild(debuff->eff);
	return debuff;
}

void orangeSkill::initAni()
{
	m_ani = Armature::create("orangeskill");
	std::string str = "";
	switch (m_level)
	{
	case LOW:
	{
		str = "skill_low";
	}
		break;
	case MID:
	{
		str = "skill_mid";
	}
		break;
	case HIGH:
	{
		str = "skill_high";
	}
		break;
	default:
		break;
	}
	m_aniName = str;
	m_ani->getAnimation()->play(str);
	this->addChild(m_ani);
}

void orangeSkill::initDir()
{
	auto ra = m_dir.getAngle();
	m_rotation = -CC_RADIANS_TO_DEGREES(ra);
	this->setRotation(m_rotation);
}

void orangeSkill::setBox()
{
	auto skillSize = Size(220, 250 + 10 * int(m_level));
	m_hitRect = BoxTool::getInstance()->getBox(this->getPosition(), this->getRotation(), skillSize);
}

void orangeSkill::update(float dt)
{
	auto rc = Rect(Vec2::ZERO, MapManager::getInstance()->getMapSize());
	if (!rc.intersectsRect(m_hitRect))
	{
		BoxCullisionManager::getInstance()->eraseHeroBox(this);
		this->unscheduleUpdate();
		this->removeFromParent();
		return;
	}
	auto nextPos = this->getPosition() + m_dir*m_speed;
	this->setPosition(nextPos);
	m_hitRect.origin += m_dir * m_speed;//box move
}