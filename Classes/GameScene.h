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
	HeroSelect* m_heroSelectLayer;
	cocos2d::TMXTiledMap* m_map;

	void initMap();
	void initHeroSelect();
};


#endif