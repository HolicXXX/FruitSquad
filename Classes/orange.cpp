#include "orange.h"

orange* orange::create()
{
	auto o = new orange();
	if (o && o->init())
	{
		o->autorelease();
		return o;
	}
	CC_SAFE_DELETE(o);
	return nullptr;
}

bool orange::init()
{
	if (!HeroBase::init())
	{
		return false;
	}
	//data
	setName("orange");
	m_hpMax = m_hp = 1000;
	resetHPBar();

	initAni();
	this->scheduleUpdate();
	return true;
}

void orange::useSkill()
{
	//TODO
}

void orange::update(float dt)
{
	if (m_speed != 0 && m_dir != Vec2::ZERO)
	{
		this->setPosition(this->getPosition() + m_dir * m_speed * dt);
	}
}

void orange::initAni()
{
	//manager data
	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/hero/orange/orange.ExportJson");
	if (m_ani == nullptr)
	{
		//callback
		m_ani = Armature::create("orange");
		m_ani->getAnimation()->play("low_stand");
		m_currentAni = "low_stand";
		this->addChild(m_ani);
	}
}

std::string orange::getAnimationName()
{
	std::string ret = "";
	char * levelStr[] = { "low", "mid", "high" };
	char * stateStr[] = { "_walk_left", "_attack", "_skill", "_stand" };
	char * attDirStr[] = { "_left", "_leftup", "_leftdown", "_down" };
	char * skillDirStr[] = { "_lr", "_up", "_down" };
	switch (m_level)
	{
	case LOW:
	{
		ret += levelStr[0];
		switch (m_baseState)
		{
		case WALK:
		{
			ret += stateStr[0];
		}
			break;
		case ATTACK:
		{
			ret += stateStr[1];
			//target
			auto degrees = CC_RADIANS_TO_DEGREES(m_target.getAngle());
			if (degrees >= 45 && degrees <= 135)
			{
				ret += attDirStr[1];
			}
			else if ((degrees >= -135 && degrees <= -105) || (degrees >= -75 && degrees <= -45))
			{
				ret += attDirStr[2];
			}
			else if (degrees > -105 && degrees < -75)
			{
				ret += attDirStr[3];
			}
			else
			{
				ret += attDirStr[0];
			}
		}
			break;
		case SKILL:
		{
			ret += stateStr[2];
			//target
			auto degrees = CC_RADIANS_TO_DEGREES(m_target.getAngle());
			if (degrees >= 45 && degrees <= 135)
			{
				ret += skillDirStr[1];
			}
			else if (degrees >= -45 && degrees <= -135)
			{
				ret += skillDirStr[2];
			}
			else
			{
				ret += skillDirStr[0];
			}
		}
			break;
		case STOP:
		{
			ret += stateStr[3];
		}
			break;
		default:
			break;
		}
	}
		break;
	case MID:
	{
		ret += levelStr[1];
		switch (m_baseState)
		{
		case WALK:
		{
			ret += stateStr[0];
		}
		break;
		case ATTACK:
		{
			ret += stateStr[1];
			//target
			auto degrees = CC_RADIANS_TO_DEGREES(m_target.getAngle());
			if (degrees >= 45 && degrees <= 135)
			{
				ret += attDirStr[1];
			}
			else if ((degrees >= -135 && degrees <= -105) || (degrees >= -75 && degrees <= -45))
			{
				ret += attDirStr[2];
			}
			else if (degrees > -105 && degrees < -75)
			{
				ret += attDirStr[3];
			}
			else
			{
				ret += attDirStr[0];
			}
		}
		break;
		case SKILL:
		{
			ret += stateStr[2];
			//target
			auto degrees = CC_RADIANS_TO_DEGREES(m_target.getAngle());
			if (degrees >= 45 && degrees <= 135)
			{
				ret += skillDirStr[1];
			}
			else if (degrees >= -45 && degrees <= -135)
			{
				ret += skillDirStr[2];
			}
			else
			{
				ret += skillDirStr[0];
			}
		}
		break;
		case STOP:
		{
			ret += stateStr[3];
		}
		break;
		default:
			break;
		}
	}
		break;
	case HIGH:
	{
		ret += levelStr[2];
		switch (m_baseState)
		{
		case WALK:
		{
			ret += stateStr[0];
		}
		break;
		case ATTACK:
		{
			ret += stateStr[1];
			//target
			auto degrees = CC_RADIANS_TO_DEGREES(m_target.getAngle());
			if (degrees >= 45 && degrees <= 135)
			{
				ret += attDirStr[1];
			}
			else if ((degrees >= -135 && degrees <= -105) || (degrees >= -75 && degrees <= -45))
			{
				ret += attDirStr[2];
			}
			else if (degrees > -105 && degrees < -75)
			{
				ret += attDirStr[3];
			}
			else
			{
				ret += attDirStr[0];
			}
		}
		break;
		case SKILL:
		{
			ret += stateStr[2];
			//target
			auto degrees = CC_RADIANS_TO_DEGREES(m_target.getAngle());
			if (degrees >= 45 && degrees <= 135)
			{
				ret += skillDirStr[1];
			}
			else if (degrees >= -45 && degrees <= -135)
			{
				ret += skillDirStr[2];
			}
			else
			{
				ret += skillDirStr[0];
			}
		}
		break;
		case STOP:
		{
			ret += stateStr[3];
		}
		break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
	return ret;
}

void orange::playAnimation(const std::string & name)
{
	if (m_ani)
	{
		m_currentAni = name;
		m_ani->getAnimation()->play(name);
	}
}

void orange::setBaseState(HeroState s)
{
	//more logic
	if (m_baseState != s)
	{
		HeroBase::setBaseState(s);
		auto name = getAnimationName();
		playAnimation(name);
	}
}

void orange::setHeroLevel(HeroLevel l)
{
	HeroBase::setHeroLevel(l);
	//
	auto name = getAnimationName();
	playAnimation(name);
}

void orange::levelUp()
{
	HeroBase::levelUp();
	//
	auto name = getAnimationName();
	playAnimation(name);
}


