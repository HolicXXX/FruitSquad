#include "orange.h"
#include "orangeSkill.h"

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
	return true;
}

void orange::useSkill()
{
	m_target = Vec2(CCRANDOM_0_1() * 10.0f - 5, CCRANDOM_0_1() * 10.0f - 5).getNormalized();
	this->setBaseState(SKILL);
}

void orange::update(float dt)
{
	if (m_speed != 0 && m_dir != Vec2::ZERO)
	{
		this->setPosition(this->getPosition() + m_dir * m_speed);
	}
}

void orange::initAni()
{
	//manager data
	if (m_ani == nullptr)
	{
		//callback
		m_ani = Armature::create("orange");
		m_ani->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(orange::aniCallFunc, this));
		m_ani->getAnimation()->play("low_stand");
		m_currentAni = "low_stand";
		this->addChild(m_ani);
	}
}

void orange::aniCallFunc(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	if (movementType == LOOP_COMPLETE)
	{
		if (m_baseState == SKILL)
		{
			SkillLevel sl;
			switch (m_level)
			{
			case LOW:
			{
				sl = SkillLevel::SKILL_LOW;
			}
				break;
			case MID:
			{
				sl = SkillLevel::SKILL_MID;
			}
				break;
			case HIGH:
			{
				sl = SkillLevel::SKILL_HIGH;
			}
				break;
			default:
				break;
			}
			auto skill = orangeSkill::create(sl, m_target);
			CCLOG("%f,%f", m_target.x, m_target.y);
			skill->setBox();
			skill->setPosition(this->getPosition());
			this->getParent()->addChild(skill, this->getZOrder());
			//add to manager
			skill->scheduleUpdate();
			this->setBaseState(m_preState);
		}
	}
}

std::string orange::getAnimationName()
{
	std::string ret = "";
	char * levelStr[] = { "low", "mid", "high" };
	char * stateStr[] = { "_walk_left", "_attack", "_skill", "_stand" };
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
			if (m_dir.x > 0)
			{
				this->setRight(true);
			}
			else
			{
				this->setRight(false);
			}
		}
			break;
		case ATTACK:
		{
			ret += stateStr[1];
			ret += getAttAniStr();
		}
			break;
		case SKILL:
		{
			ret += stateStr[2];
			ret += getSkillAniStr();
		}
			break;
		case STOP:
		{
			ret += stateStr[3];
			if (m_dir.x > 0)
			{
				this->setRight(true);
			}
			else
			{
				this->setRight(false);
			}
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
			if (m_dir.x > 0)
			{
				this->setRight(true);
			}
			else
			{
				this->setRight(false);
			}
		}
		break;
		case ATTACK:
		{
			ret += stateStr[1];
			ret += getAttAniStr();
		}
		break;
		case SKILL:
		{
			ret += stateStr[2];
			ret += getSkillAniStr();
		}
		break;
		case STOP:
		{
			ret += stateStr[3];
			if (m_dir.x > 0)
			{
				this->setRight(true);
			}
			else
			{
				this->setRight(false);
			}
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
			if (m_dir.x > 0)
			{
				this->setRight(true);
			}
			else
			{
				this->setRight(false);
			}
		}
		break;
		case ATTACK:
		{
			ret += stateStr[1];
			ret += getAttAniStr();
		}
		break;
		case SKILL:
		{
			ret += stateStr[2];
			ret += getSkillAniStr();
		}
		break;
		case STOP:
		{
			ret += stateStr[3];
			if (m_dir.x > 0)
			{
				this->setRight(true);
			}
			else
			{
				this->setRight(false);
			}
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

std::string orange::getAttAniStr()
{
	std::string ret = "";
	char * attDirStr[] = { "_left", "_leftup", "_leftdown", "_down" };
	//target
	auto degrees = -CC_RADIANS_TO_DEGREES(m_target.getAngle());
	if (degrees >= 30 && degrees < 75)
	{
		ret = attDirStr[2];
		this->setRight(true);
	}
	else if (degrees >= 105 && degrees < 150)
	{
		ret = attDirStr[2];
		this->setRight(false);
	}
	else if (degrees >= 75 && degrees < 90)
	{
		ret = attDirStr[3];
		this->setRight(true);
	}
	else if (degrees >= 90 && degrees < 105)
	{
		ret = attDirStr[3];
		this->setRight(false);
	}
	else if ((degrees >= 150 && degrees <= 180) || (degrees >= -180 && degrees <= -150))
	{
		ret = attDirStr[0];
		this->setRight(false);
	}
	else if (degrees >= -30 && degrees < 30)
	{
		ret = attDirStr[0];
		this->setRight(true);
	}
	else if (degrees >= -90 && degrees < -30)
	{
		ret = attDirStr[1];
		this->setRight(true);
	}
	else
	{
		ret = attDirStr[1];
		this->setRight(false);
	}
	return ret;
}

std::string orange::getSkillAniStr()
{
	std::string ret = "";
	char * skillDirStr[] = { "_lr", "_up", "_down" };
	//target
	auto degrees = -CC_RADIANS_TO_DEGREES(m_target.getAngle());
	if (degrees >= 45 && degrees < 90)
	{
		ret = skillDirStr[2];
		this->setRight(true);
	}
	else if (degrees >= 90 && degrees < 135)
	{
		ret = skillDirStr[2];
		this->setRight(false);
	}
	else if ((degrees >= 135 && degrees <= 180) || (degrees >= -180 && degrees < -135))
	{
		ret = skillDirStr[0];
		this->setRight(false);
	}
	else if (degrees >= -135 && degrees < -90)
	{
		ret = skillDirStr[1];
		this->setRight(false);
	}
	else if (degrees >= 90 && degrees < -45)
	{
		ret = skillDirStr[1];
		this->setRight(true);
	}
	else
	{
		ret = skillDirStr[0];
		this->setRight(true);
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
	HeroBase::setBaseState(s);
	auto name = getAnimationName();
	if (name.compare(m_currentAni) != 0)
	{	
		m_currentAni = name;
		playAnimation(m_currentAni);
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
	//level up ani
	auto name = getAnimationName();
	playAnimation(name);
}


