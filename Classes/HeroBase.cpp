#include "HeroBase.h"
#include "TargetManger.h"
#include "ui/UILoadingBar.h"
using namespace ui;

bool HeroBase::init()
{
	if (!TargetBase::init())
	{
		return false;
	}
	m_preState = m_baseState = STOP;
	m_dir = { 0, 0 };
	m_isVisi = true;
	m_isRight = false;
	m_name = "";
	m_currentAni = "";
	m_target = nullptr;

	initMoveSpeed();


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
		m_ani->setOpacity(0);
		for (auto h : m_hitVec)
		{
			h->removeFromParent();
		}
		m_hitVec.clear();
		TargetManager::getInstance()->eraseHero(this);
		//more TODO
	}
	else if (m_hp > m_hpMax)
	{
		m_hp = m_hpMax;
	}
	//
	resetHPBar();
}

void HeroBase::levelUp()
{
	if (m_level == HIGH)
		return;
	m_level = Level(int(m_level) + 1);
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
	m_speed = 2;
}

void HeroBase::getHit(Armature* eff, float demage)
{
	this->addChild(eff);
	m_hitVec.pushBack(eff);
	eff->getAnimation()->setMovementEventCallFunc(
		[this, demage,eff](Armature *armature, MovementEventType movementType, const std::string& movementID)->void{
		if (movementType == LOOP_COMPLETE)
		{
			this->hpDown(demage);
			m_hitVec.eraseObject(armature);
			eff->removeFromParent();
		}
	});
}

void HeroBase::getDeBuff(DeBuff* debuff)
{

}

void HeroBase::getBuff(Armature* eff, BuffType type, float time, float percent)
{

}

void HeroBase::pauseAll()
{
	setBaseState(STOP);
	this->unscheduleUpdate();
}

void HeroBase::resumeAll()
{
	setBaseState(m_preState);
	this->scheduleUpdate();
}

void HeroBase::pointCheck()
{
	if (m_checkPoint["name"].asString().compare("end") != 0)
	{
		auto pos = Vec2{ m_checkPoint["x"].asFloat(), m_checkPoint["y"].asFloat() };
		auto hPos = this->getPosition();
		auto dir = this->getDir();
		bool isget = false;
		if (dir.x > 0)
		{
			if (hPos.x >= pos.x)
			{
				isget = true;
			}
		}
		else if (dir.x < 0)
		{
			if (hPos.x <= pos.x)
			{
				isget = true;
			}
		}
		else
		{
			if (dir.y > 0)
			{
				if (hPos.y >= pos.y)
				{
					isget = true;
				}
			}
			else if (dir.y < 0)
			{
				if (hPos.y <= pos.y)
				{
					isget = true;
				}
			}
		}
		if (isget)
		{
			this->setPosition(pos);
			auto newdir = getNextDir();
			if (newdir.x < 0)
			{
				this->setRight(false);
			}
			else if (newdir.x > 0)
			{
				this->setRight(true);
			}
			this->setDir(newdir);
			m_checkPoint = m_changeCheckPoint(m_checkPoint);
		}
	}
	else
	{
		if (m_baseState != STOP)
		{
			auto pos = Vec2{ m_checkPoint["x"].asFloat(), m_checkPoint["y"].asFloat() };
			auto hPos = this->getPosition();
			auto dir = this->getDir();
			bool isget = false;
			if (dir.x > 0)
			{
				if (hPos.x >= pos.x)
				{
					isget = true;
				}
			}
			else if (dir.x < 0)
			{
				if (hPos.x <= pos.x)
				{
					isget = true;
				}
			}
			else
			{
				if (dir.y > 0)
				{
					if (hPos.y >= pos.y)
					{
						isget = true;
					}
				}
				else if (dir.y < 0)
				{
					if (hPos.y <= pos.y)
					{
						isget = true;
					}
				}
			}
			if (isget)
			{
				this->setPosition(pos);
				auto newdir = Vec2{ 0, 0 };
				this->setDir(newdir);
				this->setBaseState(HeroState::STOP);
			}
		}
	}		
}

Vec2 HeroBase::getNextDir()
{
	Vec2 dir = { 0, 0 };
	switch (m_checkPoint["type"].asInt())
	{
	case 1:
	{
		dir = { 1, 0 };
	}
	break;
	case 2:
	{
		dir = { 0, -1 };
	}
	break;
	case 3:
	{
		dir = { -1, 0 };
	}
	break;
	case 4:
	{
		dir = { 0, 1 };
	}
	break;
	default:
		break;
	}
	return dir;
}
