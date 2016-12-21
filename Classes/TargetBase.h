#ifndef _TARGETBASE_H_
#define _TARGETBASE_H_

#include "Level.h"
#include "Buff.h"
#include "HeadFile.h"

class TargetBase : public Node
{
public:
	virtual bool init();
	virtual void update(float dt) = 0;
	
	virtual void initAttCircle() = 0;
	virtual void initAni() = 0;
	virtual void playAnimation(const std::string & name) = 0;
	virtual std::string getAnimationName() = 0;
	virtual void bindTarget(TargetBase* t) = 0;

	virtual void pauseAll() = 0;
	virtual void resumeAll() = 0;
	virtual void getHit(Armature* eff, float demage) = 0;
	virtual void getDeBuff(DeBuff* debuff) = 0;
	virtual void getBuff(Buff* buff) = 0;
	virtual void hpDown(float d) = 0;
	virtual void resetHPBar() = 0;

	bool isAlive(){ return m_hp > 0; }

	float getAttCircleR(){ return m_attackCircleR; }
protected:
	Level m_level;
	float m_hpMax;
	float m_hp;
	float m_attDemage;
	int m_hitSpeed;
	cocos2d::Node* m_hpBar;
	std::string m_name;

	Armature* m_ani;

	TargetBase* m_target;
	Sprite* m_attCircle;
	float m_attackCircleR;

	Vector<Armature*> m_hitVec;
	Vector<Buff*> m_buffVec;
	Vector<DeBuff*> m_debuffVec;
};

#endif