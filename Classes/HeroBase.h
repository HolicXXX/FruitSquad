#ifndef _HEROBASE_H_
#define _HEROBASE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace cocostudio;

enum HeroState
{
	WALK,
	ATTACK,
	SKILL,
	STOP
};

enum HeroLevel
{
	LOW,
	MID,
	HIGH
};

class HeroBase : public cocos2d::Node
{
public:
	virtual bool init();

	virtual void useSkill() = 0;
	virtual void initAni() = 0;
	virtual void playAnimation(const std::string & name) = 0;
	virtual std::string getAnimationName() = 0;

	bool isVisi(){ return m_isVisi; }
	void setVisi(bool b);
	bool isRight(){ return m_isRight; }
	void setRight(bool b);
	void hpDown(float d);
	void resetHPBar();

	virtual void initMoveSpeed();
	virtual void setBaseState(HeroState s){ m_baseState = s; }
	HeroState getBaseState(){ return m_baseState; }
	virtual void setHeroLevel(HeroLevel l){ m_level = l; }
	HeroLevel getHeroLevel(){ return m_level; }
	virtual void levelUp();

	bool isAlive(){ return m_hp == 0; }
	void setDir(Vec2 d){ m_dir = d; }
	Vec2 getDir(){ return m_dir; }
	void setSpeed(float s){ m_speed = s; }
	float getSpeed(){ return m_speed; }
	void setName(const std::string & name){ m_name = name; }
	std::string getName(){ return m_name; }
	
protected:
	HeroState m_baseState;
	std::string m_name;
	HeroLevel m_level;
	float m_speed;
	Vec2 m_dir;
	bool m_isRight;
	bool m_isVisi;

	float m_hpMax;
	float m_hp;
	cocos2d::Node* m_hpBar;

	Armature* m_ani;
	Vec2 m_target;
	std::string m_currentAni;
};

#endif