#include "scarab.h"
#include "TargetManger.h"

scarab* scarab::create()
{
	auto s = new scarab();
	if (s && s->init())
	{
		s->autorelease();
		return s;
	}
	CC_SAFE_DELETE(s);
	return nullptr;
}

bool scarab::init()
{
	if (!EnemyBase::init())
	{
		return false;
	}
	//data
	setName("scarab");
	m_hpMax = m_hp = 2000;
	m_attDemage = 10;
	m_hitSpeed = 20;
	resetHPBar();

	m_attackCircleR = 200;
	initAttCircle();
	initAni();
	m_ani->setRotation(-90);

	this->scheduleUpdate();
	return true;
}

void scarab::initAttCircle()
{
	m_attCircle = Sprite::create("gamescene/hero/hero_skill_scope.png");
	m_attCircle->setScale(m_attackCircleR / (m_attCircle->getContentSize().width / 2));
	//m_attCircle->setColor(Color3B::YELLOW);
	m_attCircle->setVisible(false);
	this->addChild(m_attCircle);
}

void scarab::initAni()
{
	if (m_ani == nullptr)
	{
		//callback
		m_ani = Armature::create("scarab");
		//m_ani->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(orange::aniCallFunc, this));
		m_ani->getAnimation()->play("low");
		this->addChild(m_ani);
	}
}

void scarab::playAnimation(const std::string & name)
{
	if (m_ani)
	{
		m_ani->getAnimation()->play(name);
	}
}

std::string scarab::getAnimationName()
{
	std::string ret = "";
	char * levelStr[] = { "low", "mid", "high" };
	char * att = "_attack";
	switch (m_level)
	{
	case LOW:
	{
		ret = levelStr[0];
	}
		break;
	case MID:
	{
		ret = levelStr[1];
	}
		break;
	case HIGH:
	{
		ret = levelStr[2];
	}
		break;
	default:
		break;
	}
	if (m_baseState == ENEMY_ATTACK)
	{
		ret += att;
	}
	return ret;
}

void scarab::checkTargetPos()
{
	if (m_target)
	{
		auto dir = this->convertToNodeSpace(m_target->convertToWorldSpace(Vec2(0, -20)));
		if (dir.length() > m_attackCircleR || m_target->isAlive() == false)
		{
			//m_target = nullptr;
			this->setBaseState(ENEMY_STOP);
			auto tar = TargetManager::getInstance()->getHeroTarget(this);
			if (tar != nullptr)
				m_target = tar;
			return;
		}
		if (m_baseState != EnemyState::ENEMY_ATTACK)
		{
			setBaseState(EnemyState::ENEMY_ATTACK);
		}
		//auto dir = m_target->getPosition() - this->getPosition();
		auto r = -CC_RADIANS_TO_DEGREES(dir.getAngle());
		m_ani->setRotation(-90 + r);
	}
	else
	{
		if (m_baseState != ENEMY_STOP)
		{
			this->setBaseState(ENEMY_STOP);
			m_ani->setRotation(-90);
		}
		m_target = TargetManager::getInstance()->getHeroTarget(this);
	}
}

void scarab::update(float dt)
{
	for (int i = m_debuffVec.size() - 1; i >= 0; --i)
	{
		auto d = m_debuffVec.at(i);
		if (d->eff == nullptr)
		{
			d->removeFromParent();
			m_debuffVec.eraseObject(d);
		}
	}
	//buff
	checkTargetPos();
	if (m_target && m_baseState == EnemyState::ENEMY_ATTACK)
	{
		static int count = 0;
		++count;
		if (count >= m_hitSpeed)
		{
			auto pos = m_target->convertToNodeSpace(this->convertToWorldSpace(Vec2::ZERO));
			auto bul = Sprite::create("gamescene/ani/enemy/scarab/scarab_bullet.png");
			bul->setPosition(pos);
			m_target->addChild(bul);
			bul->runAction(Sequence::create(MoveTo::create(pos.length() / 1000, Vec2(0,20)),
				CallFunc::create([this, bul]()->void{
				auto arm = Armature::create("scarabBullet");
				arm->getAnimation()->play("scarabBullet");
				arm->setPosition(bul->getPosition());
				bul->removeFromParent();
				m_target->getHit(arm, m_attDemage);
			})
				, nullptr));
			count = 0;
		}
	}
}

void scarab::setBaseState(EnemyState s)
{
	//more logic
	EnemyBase::setBaseState(s);
	auto name = getAnimationName();
	playAnimation(name);
}

void scarab::setLevel(Level l)
{
	EnemyBase::setLevel(l);
	//
	auto name = getAnimationName();
	playAnimation(name);
}
