#ifndef _HEROBASE_H_
#define _HEROBASE_H_

#include "TargetBase.h"

enum HeroState
{
	WALK,
	ATTACK,
	SKILL,
	STOP
};

class HeroBase : public TargetBase
{
public:
	virtual bool init();

	virtual void initAttCircle() = 0;
	virtual void update(float dt) = 0;
	virtual bool useSkill() = 0;
	virtual void initAni() = 0;
	virtual void playAnimation(const std::string & name) = 0;
	virtual std::string getAnimationName() = 0;

	virtual void getHit(Armature* eff, float demage);
	virtual void getDeBuff(DeBuff* debuff);
	virtual void getBuff(Armature* eff, BuffType type, float time, float percent = 0);
	virtual void pauseAll();
	virtual void resumeAll();
	virtual void hpDown(float d);
	virtual void resetHPBar();
	bool isVisi(){ return m_isVisi; }
	void setVisi(bool b);
	bool isRight(){ return m_isRight; }
	void setRight(bool b);

	virtual void initMoveSpeed();
	virtual void setBaseState(HeroState s){ m_preState = m_baseState; m_baseState = s; }
	HeroState getBaseState(){ return m_baseState; }
	virtual void setHeroLevel(Level l){ m_level = l; }
	virtual Level getHeroLevel(){ return m_level; }
	virtual void levelUp();

	void setCheckPoint(ValueMap cp){ m_checkPoint = cp; }
	void bindCheckCallBack(const std::function<ValueMap(ValueMap)> & func){ m_changeCheckPoint = func; }
	void changeCheckPoint(){ m_checkPoint = m_changeCheckPoint(m_checkPoint); }
	void pointCheck();
	Vec2 getNextDir();

	void bindTarget(TargetBase* t){ m_target = t; }

	void setDir(Vec2 d){ m_dir = d; }
	Vec2 getDir(){ return m_dir; }
	void setSpeed(float s){ m_speed = s; }
	float getSpeed(){ return m_speed; }
	void setName(const std::string & name){ m_name = name; }
	std::string getName(){ return m_name; }
protected:
	HeroState m_baseState;
	HeroState m_preState;
	float m_speed;
	Vec2 m_dir;
	bool m_isRight;
	bool m_isVisi;

	std::string m_currentAni;

	ValueMap m_checkPoint;
	std::function<ValueMap(ValueMap)> m_changeCheckPoint;
};

#endif