#ifndef _START_SCENE_H_
#define _START_SCENE_H_

#include "cocos2d.h"

class StartScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	static StartScene* create();
	virtual bool init();
	virtual void onExit();

	virtual void update(float dt);
private:
	cocos2d::Sprite* m_ball;
	cocos2d::Vector<cocos2d::Sprite*> m_skyblue;
	cocos2d::Vector<cocos2d::Sprite*> m_yellow;
	cocos2d::Vector<cocos2d::Sprite*> m_blue;
	void initBG();
};


#endif