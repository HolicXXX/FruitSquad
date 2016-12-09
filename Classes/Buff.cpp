#include "Buff.h"

bool DeBuff::init()
{
	if (!Node::create)
	{
		return false;
	}

	m_delay = 0;

	return true;
}

void DeBuff::bindDemageCallBack(const std::function<void(float)>& func)
{
	m_callback = func;
}

void DeBuff::update(float dt)
{
	m_delay += dt;
	if (m_delay >= time / demageTimes)
	{
		m_delay = 0;
		m_count--;
		m_callback(demage / demageTimes);
		if (m_count == 0)
		{
			eff->removeFromParent();
			eff = nullptr;
			this->unscheduleUpdate();
		}
	}
}