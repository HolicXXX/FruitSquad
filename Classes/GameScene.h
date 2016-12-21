#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "HeroBase.h"
#include "EnemyBase.h"
#include "HeroSelectLayer.h"
#include "UIButton.h"
#include "SkillButton.h"
#include "ItemButton.h"
#include "HPBottle.h"
#include "GoldLabel.h"
#include "SettlementLayer.h"

class GameScene : public Layer
{
public:
	static Scene* createScene();
	static GameScene* create();
	virtual bool init();
	virtual void onExit();

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

	void initEnemys();
	Vector<EnemyBase*> m_enemys;

	void aniCallBack(Armature *armature, MovementEventType movementType, const std::string& movementID);
	Node* m_startEff;

	void initMap();
	float m_previewScale;
	Size m_mapSize;
	TMXTiledMap* m_map;
	void cameraMove(Vec2 step);

	HeroSelect* m_heroSelectLayer;
	void initHeroSelect();

	void initSelectMidLayer();
	Layer* m_midLayer;//ui layer
	void initUI();
	//left top ui
	UIButton* m_pause;
	UIButton* m_speed;
	void addMenuLayer();
	UIButton* m_menu;
	//right top ui
	HPBottle* m_hp;

	void getGold(Ref*);
	GoldLabel* m_gold;

	void startCallBack();

	void initItems();
	ItemButton* m_item;
	void useItem(ItemType type);

	void addDeathCount(Ref*);
	int m_deathCount;
	void callSettleMent();
	SettlementLayer* m_settleMent;
};


#endif