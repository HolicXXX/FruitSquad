#ifndef _HEROHALL_LAYER_H_
#define _HEROHALL_LAYER_H_

#include "cocos2d.h"
#include "HeroModel.h"

class HeroHall : public cocos2d::Layer
{
public:
	static HeroHall* create();
	virtual bool init();

	virtual void update(float dt);
private:
	void initHeroIcon();
	void setPropertyBox();
	void setHeroModelUI();

	HeroModel* m_heroModel;

	cocos2d::Sprite* m_returnButton;

	cocos2d::Node* m_fruits;
	cocos2d::Node* m_gems;

	cocos2d::Node* m_focus;
	cocos2d::ClippingNode* m_clipping;
	cocos2d::Vector<Node*> m_firstPage;
	cocos2d::Node* m_firstPageOrigin;
	cocos2d::Vector<Node*> m_secondPage;
	cocos2d::Node* m_secondPageOrigin;
	cocos2d::Sprite* m_leftButton;
	cocos2d::Sprite* m_rightButton;

	cocos2d::Node* m_propertyBox;

	cocos2d::Sprite* m_wcloud1;
	cocos2d::Sprite* m_wcloud2;
	cocos2d::Sprite* m_wcloud3;
	cocos2d::Sprite* m_wcloud4;
	cocos2d::Sprite* m_bcloud1;
	cocos2d::Sprite* m_bcloud2;
	cocos2d::Sprite* m_bcloud3;
	cocos2d::Sprite* m_bcloud4;
	void bgUpdate(float dt);
};


#endif