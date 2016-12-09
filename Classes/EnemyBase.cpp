#include "EnemyBase.h"
#include "TargetManger.h"
#include "ui/UILoadingBar.h"
using namespace ui;

bool EnemyBase::init()
{
	if (!TargetBase::init())
	{
		return false;
	}
	m_preState = m_baseState = ENEMY_STOP;
	m_target = nullptr;
	m_name = "";
	m_tomb = Sprite::create("gamescene/ani/enemy/mosnter_tombstone.png");
	this->addChild(m_tomb);
	m_tomb->setVisible(false);

	return true;
}

void EnemyBase::getHit(Armature* eff, float demage)
{
	this->addChild(eff);
	m_hitVec.pushBack(eff);
	this->runAction(Sequence::create(TintTo::create(0.01f, Color3B(255, 0, 0)), TintTo::create(0.01f, Color3B(255, 255, 255)), nullptr));
	eff->getAnimation()->setMovementEventCallFunc(
		[this, demage](Armature *armature, MovementEventType movementType, const std::string& movementID)->void{
		if (movementType == LOOP_COMPLETE)
		{
			if (this->isAlive() == true)
				this->hpDown(demage);
			m_hitVec.eraseObject(armature);
			armature->removeFromParent();
		}
	});
}

void EnemyBase::getDeBuff(DeBuff* debuff)
{
	bool isfind = false;
	for (auto d : m_debuffVec)
	{
		if (debuff->effName.compare(d->effName) == 0 && d->eff != nullptr)
		{
			isfind = true;
			break;
		}
	}
	if (!isfind)
	{
		m_debuffVec.pushBack(debuff);
		debuff->bindDemageCallBack(CC_CALLBACK_1(EnemyBase::hpDown, this));
		debuff->scheduleUpdate();
		this->addChild(debuff);
	}
}

void EnemyBase::getBuff(Armature* eff, BuffType type, float time, float percent)
{

}

void EnemyBase::pauseAll()
{
	setBaseState(ENEMY_STOP);
	this->unscheduleUpdate();
}

void EnemyBase::resumeAll()
{
	setBaseState(m_preState);
	this->scheduleUpdate();
}

void EnemyBase::hpDown(float d)
{
	if (m_hpBar->getOpacity() == 0)
		m_hpBar->setOpacity(255);
	m_hp -= d;
	if (m_hp < 0)
	{
		m_hp = 0;
		m_ani->setOpacity(0);
		m_tomb->setVisible(true);
		this->unscheduleUpdate();
		for (auto h : m_hitVec)
		{
			h->removeFromParent();
		}
		m_hitVec.clear();
		TargetManager::getInstance()->eraseEnemy(this);
		//gold add ani
	}
	else if (m_hp > m_hpMax)
	{
		m_hp = m_hpMax;
	}
	resetHPBar();
}

void EnemyBase::resetHPBar()
{
	if (m_hpBar == nullptr)
	{
		m_hpBar = Node::create();
		m_hpBar->setCascadeOpacityEnabled(true);
		m_hpBar->setOpacity(0);
		auto bg = Sprite::create("gamescene/hero/hp_bar_bg.png");
		bg->setTag(1);
		m_hpBar->addChild(bg);
		auto percent = m_hp / m_hpMax * 100;
		auto bar = LoadingBar::create("gamescene/hero/hp_bar_yellow.png", percent);
		bar->setDirection(LoadingBar::Direction::LEFT);
		bar->setTag(2);
		m_hpBar->addChild(bar);
		m_hpBar->setPosition(Vec2(0, 50));
		this->addChild(m_hpBar);
		return;
	}
	auto percent = m_hp / m_hpMax * 100;
	if (percent <= 0)
	{
		m_hpBar->setOpacity(0);
	}
	static_cast<LoadingBar*>(m_hpBar->getChildByTag(2))->setPercent(percent);
}