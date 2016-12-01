#ifndef _SEASON_LAYER_H_
#define _SEASON_LAYER_H_

#include "cocos2d.h"

class SeasonLayer : public cocos2d::Layer
{
public:
	static SeasonLayer* create();
	virtual bool init();

	void bindCallBack(const std::function<void(int)>& func);
private:
	int m_focusIndex;
	cocos2d::Vector<cocos2d::Node*> m_seasons;
	cocos2d::Sprite* m_backLight;
	cocos2d::Node* m_title;

	std::function<void(int)> m_callback;

	void initSeasons();
	void move(float deltaX);
	void changeFocus();
};


#endif