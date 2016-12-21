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
	void bindEraseDebuffCallBack(const std::function<void(DeBuff*)>& func);
	Armature* eff;

	DeBuffType type;
	std::string effName;
	float time;
	float demage;
private:
	std::function<void(float)> m_callback;
	std::function<void(DeBuff*)> m_erase;
	float m_delay;
};

class Buff : public Node
{
public:
	CREATE_FUNC(Buff);
	virtual bool init();
	virtual void update(float dt);

	void bindHealCallBack(const std::function<void(float)>& func);
	void bindEraseBuffCallBack(const std::function<void(Buff*)>& func);

	Armature* eff;

	BuffType type;
	std::string effName;
	float time;
	float buffNum;

private:
	std::function<void(float)> m_callback;
	std::function<void(Buff*)> m_erase;
	float m_delay;
};


#endif