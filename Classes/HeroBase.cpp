#include "HeroBase.h"
#include "ui/UILoadingBar.h"
using namespace ui;

bool HeroBase::init()
{
	if (!Node::create())
	{
		return false;
	}
	m_baseState = STOP;
	m_dir = { 0, 0 };
	m_isVisi = true;
	m_isRight = false;
	m_level = LOW;
	m_name = "";
	m_currentAni = "";
	m_hpBar = nullptr;
	m_ani = nullptr;
	m_target = { 0, 0 };

	initMoveSpeed();

	this->setCascadeOpacityEnabled(true);

	return true;
}

void HeroBase::setVisi(bool b)
{
	if (m_ani)
	{
		if (b)
		{
			m_ani->setOpacity(255);
			m_isVisi = true;
		}
		else
		{
			m_ani->setOpacity(0);
			m_isVisi = false;
		}
	}
	if (m_hpBar)
	{
		for (auto c : m_hpBar->getChildren())
		{
			c->setVisible(b);
		}
	}
}

void HeroBase::setRight(bool b)
{
	if (m_ani)
	{
		if (b)
		{
			m_ani->setScaleX(-1);
			m_isRight = true;
		}
		else
		{
			m_ani->setScaleX(1);
			m_isRight = false;
		}
	}
}

void HeroBase::hpDown(float d)
{
	m_hp -= d;
	if (m_hp < 0)
	{
		m_hp = 0;
	}
	else if (m_hp > m_hpMax)
	{
		m_hp = m_hpMax;
	}
	//
	void resetHPBar();
}

void HeroBase::levelUp()
{
	if (m_level == HIGH)
		return;
	m_level = HeroLevel(int(m_level) + 1);
}

void HeroBase::resetHPBar()
{
	if (m_hpBar == nullptr)
	{
		m_hpBar = Node::create();
		auto bg = Sprite::create("gamescene/hero/hp_bar_bg.png");
		bg->setTag(1);
		m_hpBar->addChild(bg);
		auto percent = m_hp / m_hpMax * 100;
		auto bar = LoadingBar::create("gamescene/hero/hp_bar_blue.png", percent);
		bar->setDirection(LoadingBar::Direction::LEFT);
		bar->setTag(2);
		m_hpBar->addChild(bar);
		m_hpBar->setPosition(Vec2(0, 110));
		this->addChild(m_hpBar);
		return;
	}
	auto percent = m_hp / m_hpMax * 100;
	static_cast<LoadingBar*>(m_hpBar->getChildByTag(2))->setPercent(percent);
}

void HeroBase::initMoveSpeed()
{
	//data
	m_speed = 10;
}
