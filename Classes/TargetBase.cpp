#include "TargetBase.h"

bool TargetBase::init()
{
	if (!Node::init())
	{
		return false;
	}
	this->setCascadeOpacityEnabled(true);
	m_level = LOW;
	m_ani = nullptr;
	m_hpBar = nullptr;

	return true;
}
