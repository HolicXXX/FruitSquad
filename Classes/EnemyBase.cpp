#include "EnemyBase.h"
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
		m_tomb->setVisible(true);
		//
	}
	else if (m_hp > m_hpMax)
	{
		m_hp = m_hpMax;
	}
	void resetHPBar();
}

void EnemyBase::resetHPBar()
{
	if (m_hpBar == nullptr)
	{
		m_hpBar = Node::create();
		m_hpBar->setCascadeOpacityEnabled(true);
		//m_hpBar->setOpacity(0);
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
	static_cast<LoadingBar*>(m_hpBar->getChildByTag(2))->setPercent(percent);
}