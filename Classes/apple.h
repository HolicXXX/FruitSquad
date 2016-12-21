#ifndef _APPLE_H_
#define _APPLE_H_

#include "HeroBase.h"

class apple : public HeroBase
{
public:
	static apple* create();
	virtual bool init();

	virtual void initAttCircle();
	virtual bool useSkill();
	virtual void initAni();
	virtual void playAnimation(const std::string & name);
	virtual std::string getAnimationName();

	virtual void initTouchListener();

	virtual void update(float dt);

	virtual void setBaseState(HeroState s);
	virtual void setHeroLevel(Level l);
	virtual void levelUp();
private:
	std::string getAttAniStr();
	std::string getSkillAniStr();

	void aniCallFunc(Armature *armature, MovementEventType movementType, const std::string& movementID);

	void checkTargetPos();
};


#endif