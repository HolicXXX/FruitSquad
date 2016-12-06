#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "HeroSelectLayer.h"
#include "cocostudio/CocoStudio.h"
#include "HeroBase.h"
#include "UIButton.h"
#include "SkillButton.h"
#include "HPBottle.h"
#include "GoldLabel.h"

class GameScene : public Layer
{
public:
	static Scene* createScene();
	static GameScene* create();
	virtual bool init();

	void pauseAll();
	void resumeAll();

	virtual void update(float dt);
private:
	Node* m_portal;

	ValueMap changeCheckPoint(ValueMap);

	void initHeros();
	Vector<HeroBase*> m_heros;
	HeroBase* getHero(int index);
	void initSkills();
	Vector<SkillButton*> m_skills;

	void aniCallBack(Armature *armature, MovementEventType movementType, const std::string& movementID);
	Node* m_startEff;

	void initMap();
	float m_previewScale;
	Size m_mapSize;
	TMXTiledMap* m_map;

	HeroSelect* m_heroSelectLayer;
	void initHeroSelect();

	Layer* m_midLayer;
	void initSelectMidLayer();

	void startCallBack();

	void initUI();
	UIButton* m_pause;
	UIButton* m_speed;
	UIButton* m_menu;
	void addMenuLayer();
	HPBottle* m_hp;
	GoldLabel* m_gold;
};


#endif