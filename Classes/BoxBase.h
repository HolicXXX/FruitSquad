#ifndef _BOXBASE_H_
#define _BOXBASE_H_

#include "Buff.h"

class BoxBase : public Node
{
public:
	virtual void initEff() = 0;
	virtual void setBox() = 0;
	virtual cocos2d::Rect getBox() = 0;
	virtual DeBuff* getDeBuff() = 0;
protected:
	DeBuffType m_type;
	std::string m_effName;
	float m_effDemage;
	float m_effTime;
	int m_demageTimes;
};

#endif