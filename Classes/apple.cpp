#include "apple.h"
#include "TargetManger.h"
#include "BoxCullisionManager.h"
#include "LevelUpButton.h"
#include "JsonTool.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

apple* apple::create()
{
	auto a = new apple();
	if (a && a->init())
	{
		a->autorelease();
		return a;
	}
	CC_SAFE_DELETE(a);
	return nullptr;
}

bool apple::init()
{
	if (!HeroBase::init())
	{
		return false;
	}
	setName("apple");
	m_hpMax = m_hp = 1000;
	m_attDemage = 45;
	m_hitSpeed = 15;
	resetHPBar();

	m_attackCircleR = 250;
	initAttCircle();

	initAni();
	initTouchListener();

	return true;
}

void apple::initAttCircle()
{
	m_attCircle = Sprite::create("gamescene/hero/hero_skill_scope.png");
	m_attCircle->setScale(m_attackCircleR / (m_attCircle->getContentSize().width / 2));
	m_attCircle->setColor(Color3B::GREEN);
	m_attCircle->setVisible(false);
	this->addChild(m_attCircle);
}

bool apple::useSkill()
{
	if (m_target && m_target->isAlive())
	{
		this->setBaseState(SKILL);
		return true;
	}
	return false;
}

void apple::checkTargetPos()
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

void apple::initTouchListener()
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
				b->bindLevelUpCallFunc(CC_CALLBACK_0(apple::levelUp, this));
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

void apple::update(float dt)
{
	checkTargetPos();
	if (m_target && m_baseState == ATTACK)
	{
		static int count = 0;
		++count;
		if (count >= m_hitSpeed)
		{
			auto pos = m_target->convertToNodeSpace(this->convertToWorldSpace(Vec2::ZERO));
			char* bulStr[] = { "low_", "mid_", "high_" };
			auto bul = Sprite::create(StringUtils::format("gamescene/ani/hero/apple/%sbullet.png", bulStr[int(m_level)]));
			bul->setPosition(pos);
			bul->runAction(Sequence::create(
				MoveTo::create(0.3f, Vec2(0, 0)),
				CallFunc::create([this, bul]()->void{
				auto buff = Buff::create();//defend down buff
				buff->type = BuffType::DEMAGE_AVOID;
				buff->effName = "none";
				buff->time = 2.0f;
				buff->buffNum = -10.0f;
				static_cast<EnemyBase*>(bul->getParent())->getBuff(buff);
				static_cast<EnemyBase*>(bul->getParent())->hpDown(m_attDemage);
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

void apple::initAni()
{
	if (m_ani == nullptr)
	{
		m_ani = Armature::create("apple");
		m_ani->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(apple::aniCallFunc, this));
		m_ani->getAnimation()->play("low_stand");
		m_currentAni = "low_stand";
		this->addChild(m_ani);
	}
}

void apple::aniCallFunc(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	if (movementType == LOOP_COMPLETE)
	{
		if (m_baseState == SKILL)
		{
			auto dir = this->convertToNodeSpace(m_target->convertToWorldSpace(Vec2::ZERO)).getNormalized();
			auto skill = Armature::create("appleSkill");
			char* skillStr[] = { "skill_low", "skill_mid", "skill_high" };
			skill->getAnimation()->play(skillStr[int(m_level)]);
			if (JsonTool::getInstance()->getDoc()["sound"].GetBool())
				SimpleAudioEngine::getInstance()->playEffect("sound/apple_skill.wav");
			skill->getAnimation()->setMovementEventCallFunc(
				[](Armature *armature, MovementEventType movementType, const std::string& movementID){
				if (movementType == LOOP_COMPLETE)
				{
					armature->removeFromParent();
				}
			});
			skill->setPosition(m_target->getPosition());
			this->getParent()->addChild(skill, this->getZOrder());
			this->setBaseState(m_preState);
			m_target->hpDown(500);//skill demage data
		}
	}
}

std::string apple::getAnimationName()
{
	std::string ret = "";
	char * levelStr[] = { "low", "mid", "high" };
	char * stateStr[] = { "_walk", "_attack", "_stand", "_skill" };
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
			ret += stateStr[3];
			auto dir = this->convertToNodeSpace(m_target->convertToWorldSpace(Vec2::ZERO)).getNormalized();
			if (dir.x > 0)
			{
				this->setRight(true);
			}
			else
			{
				this->setRight(false);
			}
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
			ret += getAttAniStr();
		}
		break;
		case SKILL:
		{
			ret += stateStr[3];
			auto dir = this->convertToNodeSpace(m_target->convertToWorldSpace(Vec2::ZERO)).getNormalized();
			if (dir.x > 0)
			{
				this->setRight(true);
			}
			else
			{
				this->setRight(false);
			}
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
			ret += getAttAniStr();
		}
		break;
		case SKILL:
		{
			ret += stateStr[3];
			auto dir = this->convertToNodeSpace(m_target->convertToWorldSpace(Vec2::ZERO)).getNormalized();
			if (dir.x > 0)
			{
				this->setRight(true);
			}
			else
			{
				this->setRight(false);
			}
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

std::string apple::getAttAniStr()
{
	std::string ret = "";
	char * attDirStr[] = { "_lrd", "_up"};
	auto dir = this->convertToNodeSpace(m_target->convertToWorldSpace(Vec2::ZERO)).getNormalized();
	if (dir.x > 0)
	{
		this->setRight(true);
	}
	else
	{
		this->setRight(false);
	}
	auto degrees = -CC_RADIANS_TO_DEGREES(dir.getAngle());
	if (degrees >= -135 && degrees < -45)
	{
		ret = attDirStr[1];
	}
	else
	{
		ret = attDirStr[0];
	}
	return ret;
}

std::string apple::getSkillAniStr()
{
	std::string ret = "";
	/*char * skillDirStr[] = { "_low", "_mid", "_high" };
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
	}*/
	return ret;
}

void apple::playAnimation(const std::string & name)
{
	if (m_ani)
	{
		m_currentAni = name;
		m_ani->getAnimation()->play(name);
	}
}

void apple::setBaseState(HeroState s)
{
	HeroBase::setBaseState(s);
	auto name = getAnimationName();
	if (name.compare(m_currentAni) != 0)
	{
		m_currentAni = name;
		playAnimation(m_currentAni);
	}
}

void apple::setHeroLevel(Level l)
{
	HeroBase::setHeroLevel(l);
	auto name = getAnimationName();
	playAnimation(name);
	m_attackCircleR = 200 + 50 * int(l);
	m_attCircle->setScale(m_attackCircleR / (m_attCircle->getContentSize().width / 2));
	m_attDemage = 45 + 10 * int(l);
}

void apple::levelUp()
{
	HeroBase::levelUp();
	auto name = getAnimationName();
	playAnimation(name);
	m_attackCircleR = 200 + 50 * int(m_level);
	m_attCircle->setScale(m_attackCircleR / (m_attCircle->getContentSize().width / 2));
	m_attDemage = 45 + 10 * int(m_level);
}




