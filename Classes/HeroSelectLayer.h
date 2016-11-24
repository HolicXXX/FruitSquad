#ifndef _HEROSELECT_LAYER_H_
#define _HEROSELECT_LAYER_H_

#include "cocos2d.h"
class HeroSelect : public cocos2d::Layer
{
public:
	static HeroSelect* create();
	virtual bool init();
	virtual void update(float dt);
private:
	cocos2d::Sprite* m_returnButton;

	void initTitle();
	cocos2d::Node* m_title;
	
	void initItemBG();
	cocos2d::Node* m_itemSelect;
	cocos2d::Sprite* m_previewButton;

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