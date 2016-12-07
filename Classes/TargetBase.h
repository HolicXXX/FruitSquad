#ifndef _TARGETBASE_H_
#define _TARGETBASE_H_

#include "Level.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace cocostudio;

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
	virtual void hpDown(float d) = 0;
	virtual void resetHPBar() = 0;

	bool isAlive(){ return m_hp == 0; }

	float getAttCircleR(){ return m_attackCircleR; }

protected:
	Level m_level;
	float m_hpMax;
	float m_hp;
	cocos2d::Node* m_hpBar;
	std::string m_name;

	Armature* m_ani;

	TargetBase* m_target;
	Sprite* m_attCircle;
	float m_attackCircleR;
};

#endif