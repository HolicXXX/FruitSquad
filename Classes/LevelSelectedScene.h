#ifndef _LEVELSELECTED_SCENE_H_
#define _LEVELSELECTED_SCENE_H_

#include "cocos2d.h"
#include "LevelButton.h"
#include "HeartPoints.h"
#include "HeroHallLayer.h"
#include "ShopLayer.h"

class LevelSelectedScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	static LevelSelectedScene* create();
	virtual bool init();
	virtual void onExit();

	virtual void update(float dt);
private:
	cocos2d::Sprite* m_background;

	cocos2d::Sprite* m_leftButton;
	cocos2d::Sprite* m_rightButton;
	cocos2d::ClippingNode* m_cliping;
	LevelButton* m_focus;
	cocos2d::Vector<LevelButton*> m_firstpage;
	cocos2d::Node* m_firstMid;
	cocos2d::Vector<LevelButton*> m_secondpage;
	cocos2d::Node* m_secondMid;

	cocos2d::MenuItemImage* m_home;

	HeartPoints* m_heartPoints;
	cocos2d::Node* m_gems;
	cocos2d::Node* m_keys;
	//layers
	HeroHall* m_heroHallLayer;
	ShopLayer* m_shopLayer;

	cocos2d::MenuItemImage* m_seasons;
	cocos2d::MenuItemImage* m_heroHall;
	cocos2d::MenuItemImage* m_shop;

	void initLevelButton();
	void initUI();

	void gameStart();
};


#endif