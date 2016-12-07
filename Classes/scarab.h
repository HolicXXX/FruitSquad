#ifndef _SCARAB_H_
#define _SCARAB_H_

#include "EnemyBase.h"

class scarab : public EnemyBase
{
public:
	static scarab* create();
	virtual bool init();

	virtual void initAttCircle();
	virtual void update(float dt);
	virtual void initAni();
	virtual void playAnimation(const std::string & name);
	virtual std::string getAnimationName();

	virtual void setBaseState(EnemyState s);
	virtual void setLevel(Level l);

private:

	void checkTargetPos();
};


#endif