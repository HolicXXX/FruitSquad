#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "HeroSelectLayer.h"

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	static GameScene* create();
	virtual bool init();

	virtual void update(float dt);
private:
	cocos2d::Node* m_portal;

	void initMap();
	cocos2d::TMXTiledMap* m_map;

	HeroSelect* m_heroSelectLayer;
	void initHeroSelect();

	cocos2d::Layer* m_midLayer;
	void initSelectMidLayer();

	void startCallBack();
};


#endif