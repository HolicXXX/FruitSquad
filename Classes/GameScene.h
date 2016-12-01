#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "HeroSelectLayer.h"
#include "cocostudio/CocoStudio.h"
#include "HeroBase.h"

class GameScene : public Layer
{
public:
	static Scene* createScene();
	static GameScene* create();
	virtual bool init();

	virtual void update(float dt);
private:
	Node* m_portal;

	void initHeros();
	Vector<HeroBase*> m_heros;

	void aniCallBack(Armature *armature, MovementEventType movementType, const std::string& movementID);
	Node* m_startEff;

	void initMap();
	TMXTiledMap* m_map;

	HeroSelect* m_heroSelectLayer;
	void initHeroSelect();

	Layer* m_midLayer;
	void initSelectMidLayer();

	void startCallBack();

	void initUI();

};


#endif