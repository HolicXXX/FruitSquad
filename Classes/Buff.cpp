#include "Buff.h"

bool DeBuff::init()
{
	if (!Node::init())
	{
		return false;
	}
	this->setCascadeOpacityEnabled(true);
	m_delay = 0;

	return true;
}

void DeBuff::bindDemageCallBack(const std::function<void(float)>& func)
{
	m_callback = func;
}

void DeBuff::bindEraseDebuffCallBack(const std::function<void(DeBuff*)>& func)
{
	m_erase = func;
}

void DeBuff::update(float dt)
{
	m_delay += dt;
	m_callback(dt / time * demage);
	if (m_delay >= time)
	{
		m_delay = 0;
		m_erase(this);
		this->unscheduleUpdate();
		if (type == FREEZE)
		{
			eff->getAnimation()->play("melt");
		}
		else
		{
			this->removeFromParent();
		}
	}
}


bool Buff::init()
{
	if (!Node::init())
	{
		return false;
	}
	this->setCascadeOpacityEnabled(true);
	m_delay = 0;

	return true;
}

void Buff::update(float dt)
{
	m_delay += dt;
	if (type == BuffType::HEAL)
	{
		m_callback(-(dt / time) * buffNum);
	}
	if (m_delay >= time)
	{
		m_delay = 0;
		m_erase(this);
		this->unscheduleUpdate();
		this->removeFromParent();
	}
}

void Buff::bindHealCallBack(const std::function<void(float)>& func)
{
	m_callback = func;
}

void Buff::bindEraseBuffCallBack(const std::function<void(Buff*)>& func)
{
	m_erase = func;
}
