#ifndef _HEART_POINTS_H_
#define _HEART_POINTS_H_

#include "cocos2d.h"

class HeartPoints : public cocos2d::Node
{
public:
	static HeartPoints* create();
	virtual bool init();
	virtual void update(float dt);

private:
	cocos2d::Sprite* m_hpBG;
	cocos2d::Vector<cocos2d::Sprite*> m_hearts;

	void initHearts();
};


#endif