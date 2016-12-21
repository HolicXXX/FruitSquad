#ifndef _SETTLEMENT_LAYER_H_
#define _SETTLEMENT_LAYER_H_

#include "ScalableBar.h"
#include "SettlementStar.h"
#include "SettlementFruit.h"
#include "UIButton.h"

class SettlementLayer : public cocos2d::Layer
{
public:
	static SettlementLayer* create(int deathtimes);
	virtual bool init(int deathtimes);
	virtual void onExit();

	void bindRetryCallBack(const std::function<void()>& func);
	void bindNextCallBack(const std::function<void()>& func);
	
	int getExtraFruitNum();
	int getExtraStarNum();
private:
	int m_deathTimes;
	void settle(Ref*);

	cocos2d::Sprite* m_title;
	cocos2d::Sprite* m_key;

	cocos2d::Label* m_keyLabel;
	cocos2d::Label* m_gemLabel;

	void initBG();
	cocos2d::Sprite* m_bg;
	cocos2d::Sprite* m_eff;

	void initHeroHallButton();
	cocos2d::Node* m_herohallButton;

	void initStar();
	SettlementStar* m_stars;
	int m_getStarNum;

	void initGoalBar();
	ScalableBar* m_goal;
	void goalCallBack1();
	void goalCallBack2();
	void goalCallBack3();

	void initFruits();
	cocos2d::Vector<SettlementFruit*> m_fruits;
	int m_getFruitNum;

	void initButton();
	UIButton* m_retry;
	UIButton* m_next;
};


#endif