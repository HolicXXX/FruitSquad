#ifndef _HEROSELECT_LAYER_H_
#define _HEROSELECT_LAYER_H_

#include "cocos2d.h"
#include "TeamList.h"
#include "MapLevelStar.h"
class HeroSelect : public cocos2d::Layer
{
public:
	static HeroSelect* create();
	virtual bool init();

	void bindStartCallBack(const std::function<void()> & func){ m_startcallback = func; }

	virtual void update(float dt);
private:
	cocos2d::Sprite* m_returnButton;
	MapLevelStar* m_star;
	std::function<void()> m_startcallback;

	void initTeamList();
	TeamList* m_teamList;

	void initTitle();
	cocos2d::Node* m_title;
	
	void initItemBG();
	cocos2d::Node* m_itemSelect;
	cocos2d::Sprite* m_previewButton;

	void initStartButton();
	void setStartButtonState(bool can);
	cocos2d::Node* m_startButton;

	void initHeroIcon();
	cocos2d::Node* m_focus;
	cocos2d::ClippingNode* m_clipping;
	cocos2d::Vector<Node*> m_firstPage;
	cocos2d::Node* m_firstPageOrigin;
	cocos2d::Vector<Node*> m_secondPage;
	cocos2d::Node* m_secondPageOrigin;
	cocos2d::Sprite* m_leftButton;
	cocos2d::Sprite* m_rightButton;


	cocos2d::Sprite* m_wcloud1;
	cocos2d::Sprite* m_wcloud2;
	cocos2d::Sprite* m_wcloud3;
	cocos2d::Sprite* m_wcloud4;
	cocos2d::Sprite* m_bcloud1;
	cocos2d::Sprite* m_bcloud2;
	cocos2d::Sprite* m_bcloud3;
	cocos2d::Sprite* m_bcloud4;
	void initBackGround();
	void bgUpdate(float dt);
};

#endif