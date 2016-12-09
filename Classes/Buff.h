#ifndef _BUFF_H_
#define _BUFF_H_

#include "HeadFile.h"

enum BuffType
{
	HEAL,
	DEMAGE_AVOID,
	DEMAGE_INCREASE,
	SPEED_UP,
};

enum DeBuffType
{
	BURN,
	FREEZE,
	SLOW_DOWN
};

class DeBuff : public Node
{
public:
	CREATE_FUNC(DeBuff);
	virtual bool init();
	virtual void update(float dt);
	void bindDemageCallBack(const std::function<void(float)>& func);
	Armature* eff;

	DeBuffType type;
	std::string effName;
	float time;
	float demage;
	int demageTimes;

	int m_count;
private:
	std::function<void(float)> m_callback;

	float m_delay;
};


#endif