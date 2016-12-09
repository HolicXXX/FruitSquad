#include "orange.h"
#include "orangeSkill.h"
#include "TargetManger.h"
#include "BoxCullisionManager.h"

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
	m_attDemage = 60;
	m_hitSpeed = 10;
	resetHPBar();

	m_attackCircleR = 200;
	initAttCircle();

	initAni();
	return true;
}

void orange::initAttCircle()
{
	m_attCircle = Sprite::create("gamescene/hero/hero_skill_scope.png");
	m_attCircle->setScale(m_attackCircleR / (m_attCircle->getContentSize().width / 2));
	m_attCircle->setColor(Color3B::GREEN);
	this->addChild(m_attCircle);
}

bool orange::useSkill()
{
	if (m_target && m_target->isAlive())
	{
		this->setBaseState(SKILL);
		return true;
	}
	return false;
}

void orange::checkTargetPos()
{
	if (m_baseState == HeroState::SKILL)
	{
		return;
	}
	if (m_target)
	{
		auto dir = this->convertToNodeSpace(m_target->convertToWorldSpace(Vec2::ZERO));
		if (dir.length() > m_attackCircleR || m_target->isAlive() == false)
		{
			m_target = nullptr;
			if (m_baseState != WALK)
			{
				setBaseState(WALK);
			}
			m_target = TargetManager::getInstance()->getEnemyTarget(this);
		}
		if (m_baseState != HeroState::ATTACK && m_target)
		{
			setBaseState(HeroState::ATTACK);
		}
		else
		{
			auto name = getAnimationName();
			if (name.compare(m_currentAni) != 0)
			{
				m_currentAni = name;
				playAnimation(m_currentAni);
			}
		}
	}
	else
	{
		if (m_baseState != HeroState::WALK && m_baseState != HeroState::STOP)
		{
			setBaseState(HeroState::WALK);
		}
		m_target = TargetManager::getInstance()->getEnemyTarget(this);
	}
}

void orange::update(float dt)
{
	checkTargetPos();
	if (m_target && m_baseState == ATTACK)
	{	
		static int count = 0;
		++count;
		if (count >= m_hitSpeed)
		{
			auto arm = Armature::create("orangeBulletEff");
			arm->getAnimation()->play("orangeBulletEff");
			m_target->getHit(arm, m_attDemage);
			count = 0;
		}
	}
	if (m_speed != 0 && m_dir != Vec2::ZERO)
	{
		this->setPosition(this->getPosition() + m_dir * m_speed);
	}
}

void orange::initAni()
{
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
			auto dir = this->convertToNodeSpace(m_target->convertToWorldSpace(Vec2::ZERO)).getNormalized();
			auto skill = orangeSkill::create(m_level, dir);
			this->getParent()->addChild(skill, this->getZOrder());
			skill->setPosition(this->getPosition());
			skill->setBox();
			BoxCullisionManager::getInstance()->addHeroBox(skill);
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
			else if (m_dir.x < 0)
			{
				this->setRight(false);
			}
			else
			{
				this->setRight(m_isRight);
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
			else if (m_dir.x < 0)
			{
				this->setRight(false);
			}
			else
			{
				this->setRight(m_isRight);
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
			else if (m_dir.x < 0)
			{
				this->setRight(false);
			}
			else
			{
				this->setRight(m_isRight);
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
	auto dir = this->convertToNodeSpace(m_target->convertToWorldSpace(Vec2::ZERO)).getNormalized();
	auto degrees = -CC_RADIANS_TO_DEGREES(dir.getAngle());
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
	auto dir = this->convertToNodeSpace(m_target->convertToWorldSpace(Vec2::ZERO)).getNormalized();
	auto degrees = -CC_RADIANS_TO_DEGREES(dir.getAngle());
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

void orange::setHeroLevel(Level l)
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


