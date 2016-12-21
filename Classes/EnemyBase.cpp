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
	if (this->isAlive() == false)
		return;
	this->addChild(eff);
	m_hitVec.pushBack(eff);
	this->hpDown(demage);
	eff->getAnimation()->setMovementEventCallFunc(
		[this](Armature *armature, MovementEventType movementType, const std::string& movementID)->void{
		if (movementType == LOOP_COMPLETE)
		{
			m_hitVec.eraseObject(armature);
			armature->removeFromParent();
		}
	});
}

void EnemyBase::eraseDebuff(DeBuff* d)
{
	if (this->isAlive())
		m_debuffVec.eraseObject(d);
}

void EnemyBase::eraseBuff(Buff* b)
{
	if (this->isAlive())
		m_buffVec.eraseObject(b);
}

void EnemyBase::getDeBuff(DeBuff* debuff)
{
	bool isfind = false;
	for (auto d : m_debuffVec)
	{
		if (debuff->effName.compare(d->effName) == 0 && debuff->type == d->type)
		{
			isfind = true;
			break;
		}
	}
	if (!isfind)
	{
		m_debuffVec.pushBack(debuff);
		debuff->bindDemageCallBack(CC_CALLBACK_1(EnemyBase::hpDown, this));
		debuff->bindEraseDebuffCallBack(CC_CALLBACK_1(EnemyBase::eraseDebuff, this));
		debuff->scheduleUpdate();
		this->addChild(debuff);
	}
}

void EnemyBase::getBuff(Buff* buff)
{
	bool isfind = false;
	for (auto b : m_buffVec)
	{
		if (buff->effName.compare(b->effName) == 0 && buff->type == b->type)
		{
			isfind = true;
			break;
		}
	}
	if (!isfind)
	{
		m_buffVec.pushBack(buff);
		buff->bindHealCallBack(CC_CALLBACK_1(EnemyBase::hpDown, this));
		buff->bindEraseBuffCallBack(CC_CALLBACK_1(EnemyBase::eraseBuff, this));
		buff->scheduleUpdate();
		this->addChild(buff);
	}
}

void EnemyBase::pauseAll()
{
	setBaseState(ENEMY_STOP);
	this->unscheduleUpdate();
	for (auto d : m_debuffVec)
	{
		d->unscheduleUpdate();
	}
	for (auto b : m_buffVec)
	{
		b->unscheduleUpdate();
	}
}

void EnemyBase::resumeAll()
{
	setBaseState(m_preState);
	this->scheduleUpdate();
	for (auto d : m_debuffVec)
	{
		d->scheduleUpdate();
	}
	for (auto b : m_buffVec)
	{
		b->scheduleUpdate();
	}
}

void EnemyBase::hpDown(float d)
{
	if (isAlive() == false)
		return;
	if (m_hpBar->getOpacity() == 0)
		m_hpBar->setOpacity(255);
	if (d > 0)
	{
		float demagePercent = 1;
		for (auto b : m_buffVec)
		{
			if (b->type == BuffType::DEMAGE_AVOID)
				demagePercent *= ((100 - b->buffNum) / 100);
		}
		m_hp -= d * demagePercent;
	}
	else
	{
		m_hp -= d / 100.0f * m_hpMax;
	}

	if (m_hp <= 0)
	{
		m_hp = 0;
		m_ani->setOpacity(0);
		m_tomb->setVisible(true);
		for (auto h : m_hitVec)
		{
			h->setOpacity(0);
		}
		m_hitVec.clear();
		for (auto d : m_debuffVec)
		{
			d->setOpacity(0);
		}
		m_debuffVec.clear();
		for (auto b : m_buffVec)
		{
			b->setOpacity(0);
		}
		m_buffVec.clear();
		TargetManager::getInstance()->eraseEnemy(this);
		NotificationCenter::getInstance()->postNotification("addGoldAni", this);
		this->unscheduleUpdate();
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