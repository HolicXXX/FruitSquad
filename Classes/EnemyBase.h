#ifndef _ENEMYBASE_H_
#define _ENEMYBASE_H_

#include "TargetBase.h"

enum EnemyState
{
	ENEMY_STOP,
	ENEMY_ATTACK
};

class EnemyBase : public TargetBase
{
public:
	virtual bool init();

	virtual void initAttCircle() = 0;
	virtual void update(float dt) = 0;
	virtual void initAni() = 0;
	virtual void playAnimation(const std::string & name) = 0;
	virtual std::string getAnimationName() = 0;

	virtual void pauseAll();
	virtual void resumeAll();
	virtual void hpDown(float d);
	virtual void resetHPBar();

	void bindTarget(TargetBase* t){ m_target = t; }

	virtual void setBaseState(EnemyState s){ m_preState = m_baseState; m_baseState = s; }
	EnemyState getBaseState(){ return m_baseState; }
	virtual void setLevel(Level l){ m_level = l; }
	Level getLevel(){ return m_level; }

	void setName(const std::string & name){ m_name = name; }
	std::string getName(){ return m_name; }
protected:
	EnemyState m_baseState;
	EnemyState m_preState;

	Sprite* m_tomb;
};



#endif