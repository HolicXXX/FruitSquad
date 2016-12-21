#include "coconut.h"
#include "TargetManger.h"
#include "BoxCullisionManager.h"
#include "LevelUpButton.h"
#include "JsonTool.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

coconut* coconut::create()
{
	auto c = new coconut();
	if (c && c->init())
	{
		c->autorelease();
		return c;
	}
	CC_SAFE_DELETE(c);
	return nullptr;
}

bool coconut::init()
{
	if (!HeroBase::init())
	{
		return false;
	}
	setName("orange");
	m_hpMax = m_hp = 1000;
	m_attDemage = 30;
	m_hitSpeed = 15;
	resetHPBar();

	m_attackCircleR = 200;
	initAttCircle();

	initAni();
	initTouchListener();

	return true;
}

void coconut::initAttCircle()
{
	m_attCircle = Sprite::create("gamescene/hero/hero_skill_scope.png");
	m_attCircle->setScale(m_attackCircleR / (m_attCircle->getContentSize().width / 2));
	m_attCircle->setColor(Color3B::GREEN);
	m_attCircle->setVisible(false);
	this->addChild(m_attCircle);
}

bool coconut::useSkill()
{
	this->setBaseState(SKILL);
	return true;
}

void coconut::checkTargetPos()
{
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

void coconut::initTouchListener()
{
	Size aniSize = { 80, 100 };
	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [this, aniSize](Touch* t, Event* e)->bool{
		auto pos = this->convertTouchToNodeSpace(t);
		auto rc = Rect(Vec2(0 - aniSize.width / 2, 0), aniSize);
		if (rc.containsPoint(pos))
		{
			return true;
		}
		else
		{
			m_attCircle->setVisible(false);
			auto button = this->getChildByName("levelUpButton");
			if (button != nullptr)
			{
				auto b = static_cast<LevelUpButton*>(button);
				NotificationCenter::getInstance()->removeObserver(b, "resetLevelUpButton");
				this->removeChild(b);
			}
			return false;
		}
	};
	lis->onTouchEnded = [this, aniSize](Touch* t, Event* e)->void{
		auto pos = this->convertTouchToNodeSpace(t);
		auto rc = Rect(Vec2(0 - aniSize.width / 2, 0), aniSize);
		if (rc.containsPoint(pos))
		{
			m_attCircle->setVisible(true);
			auto button = this->getChildByName("levelUpButton");
			if (button == nullptr)
			{
				int cost = JsonTool::getInstance()->getDoc()["heros"][0]["upgrade"].GetInt() * powf(2, int(m_level));//as member
				auto b = LevelUpButton::create(m_level, cost);
				b->setPosition(Vec2(0, 130));
				b->setName("levelUpButton");
				b->bindLevelUpCallFunc(CC_CALLBACK_0(coconut::levelUp, this));
				this->addChild(b);
			}
		}
		else
		{
			m_attCircle->setVisible(false);
			auto button = this->getChildByName("levelUpButton");
			if (button != nullptr)
			{
				auto b = static_cast<LevelUpButton*>(button);
				NotificationCenter::getInstance()->removeObserver(b, "resetLevelUpButton");
				this->removeChild(b);
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, this);
}

void coconut::update(float dt)
{
	checkTargetPos();
	if (m_target && m_baseState == ATTACK)
	{
		static int count = 0;
		++count;
		if (count >= m_hitSpeed)
		{
			auto pos = m_target->convertToNodeSpace(this->convertToWorldSpace(Vec2::ZERO));
			auto bul = Armature::create("coconutBulletEff");
			bul->setPosition(pos);
			bul->getAnimation()->play("bullet");
			auto ra = -CC_RADIANS_TO_DEGREES((-pos).getAngle());
			bul->setRotation(ra);
			bul->runAction(Sequence::create(
				MoveTo::create(0.3f, Vec2(0, 0)),
				CallFunc::create([this, bul]()->void{
				auto eff = Armature::create("coconutBulletEff");
				eff->getAnimation()->play("eff");
				static_cast<EnemyBase*>(bul->getParent())->getHit(eff, m_attDemage);
				if (JsonTool::getInstance()->getDoc()["sound"].GetBool())
					SimpleAudioEngine::getInstance()->playEffect("sound/coconut_attack.wav");
				bul->removeFromParent();
			}),
				nullptr));
			m_target->addChild(bul);
			count = 0;
		}
	}
	if (m_speed != 0 && m_dir != Vec2::ZERO)
	{
		this->setPosition(this->getPosition() + m_dir * m_speed);
		if (m_cameraMove)
			m_cameraMove(m_dir * m_speed);
	}
}

void coconut::initAni()
{
	if (m_ani == nullptr)
	{
		m_ani = Armature::create("coconut");
		m_ani->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(coconut::aniCallFunc, this));
		m_ani->getAnimation()->play("low_stand");
		m_currentAni = "low_stand";
		this->addChild(m_ani);
	}
}

void coconut::aniCallFunc(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	if (movementType == LOOP_COMPLETE)
	{
		//if (m_baseState == SKILL)
		//{
		//	auto dir = this->convertToNodeSpace(m_target->convertToWorldSpace(Vec2::ZERO)).getNormalized();
		//	/*auto skill = orangeSkill::create(m_level, dir);
		//	this->getParent()->addChild(skill, this->getZOrder());
		//	skill->setPosition(this->getPosition());
		//	skill->setBox();
		//	BoxCullisionManager::getInstance()->addHeroBox(skill);
		//	skill->scheduleUpdate();
		//	this->setBaseState(m_preState);*/
		//}
	}
}

std::string coconut::getAnimationName()
{
	std::string ret = "";
	char * levelStr[] = { "low", "mid", "high" };
	char * stateStr[] = { "_walk", "_attack", "_stand" };
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
			getAttAniStr();
		}
		break;
		case SKILL:
		{
		}
		break;
		case STOP:
		{
			ret += stateStr[2];
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
			getAttAniStr();
		}
		break;
		case SKILL:
		{
		}
		break;
		case STOP:
		{
			ret += stateStr[2];
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
			getAttAniStr();
		}
		break;
		case SKILL:
		{
		}
		break;
		case STOP:
		{
			ret += stateStr[2];
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

std::string coconut::getAttAniStr()
{
	std::string ret = "";
	//char * attDirStr[] = { "_left", "_leftup", "_leftdown", "_down" };
	////target
	auto dir = this->convertToNodeSpace(m_target->convertToWorldSpace(Vec2::ZERO)).getNormalized();
	if (dir.x > 0)
	{
		this->setRight(true);
	}
	else
	{
		this->setRight(false);
	}
	return ret;
}

std::string coconut::getSkillAniStr()
{
	std::string ret = "skillBuff";
	char * skillDirStr[] = { "_low", "_mid", "_high" };
	switch (m_level)
	{
	case LOW:
	{
		ret += skillDirStr[0];
	}
		break;
	case MID:
	{
		ret += skillDirStr[1];
	}
		break;
	case HIGH:
	{
		ret += skillDirStr[2];
	}
		break;
	default:
		break;
	}
	return ret;
}

void coconut::playAnimation(const std::string & name)
{
	if (m_ani)
	{
		m_currentAni = name;
		m_ani->getAnimation()->play(name);
	}
}

void coconut::setBaseState(HeroState s)
{
	if (s == HeroState::SKILL)
	{
		auto skill = Armature::create("coconutSkillEff");
		skill->getAnimation()->play("skillEff");
		if (JsonTool::getInstance()->getDoc()["sound"].GetBool())
			SimpleAudioEngine::getInstance()->playEffect("sound/coconut_skill.wav");
		skill->getAnimation()->setMovementEventCallFunc(
			[this](Armature *armature, MovementEventType movementType, const std::string& movementID)->void{
			if (movementType == LOOP_COMPLETE)
			{
				armature->removeFromParent();
				auto hvec = TargetManager::getInstance()->getHeroTargets();
				for (auto h : hvec)
				{
					if (h->isAlive())
					{
						auto buff = Buff::create();
						buff->type = BuffType::DEMAGE_AVOID;
						buff->time = 5.0f;
						buff->buffNum = 75.0f;
						buff->effName = "coconutSkillBuff";
						buff->eff = Armature::create(buff->effName);
						buff->eff->getAnimation()->play(this->getSkillAniStr());
						buff->addChild(buff->eff);
						buff->setPosition(Vec2(0, 40));
						h->getBuff(buff);
					}
				}
			}
		});
		this->addChild(skill);
		return;
	}
	HeroBase::setBaseState(s);
	auto name = getAnimationName();
	if (name.compare(m_currentAni) != 0)
	{
		m_currentAni = name;
		playAnimation(m_currentAni);
	}
}

void coconut::setHeroLevel(Level l)
{
	HeroBase::setHeroLevel(l);
	auto name = getAnimationName();
	playAnimation(name);
	m_attackCircleR = 200 + 50 * int(l);
	m_attCircle->setScale(m_attackCircleR / (m_attCircle->getContentSize().width / 2));
	m_attDemage = 30 + 5 * int(l);
}

void coconut::levelUp()
{
	HeroBase::levelUp();
	auto name = getAnimationName();
	playAnimation(name);
	m_attackCircleR = 200 + 50 * int(m_level);
	m_attCircle->setScale(m_attackCircleR / (m_attCircle->getContentSize().width / 2));
	m_attDemage = 30 + 5 * int(m_level);
}

