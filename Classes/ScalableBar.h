#ifndef _SCALABLEBAR_H_
#define _SCALABLEBAR_H_

#include "cocos2d.h"

class ScalableBar : public cocos2d::Node
{
public:
	static ScalableBar* create(float maxPercent);
	virtual bool init(float maxPercent);
	virtual void update(float dt);

	void bindCallBack1(const std::function<void()>& func){ m_callback1 = func; }
	void bindCallBack2(const std::function<void()>& func){ m_callback2 = func; }
	void bindCallBack3(const std::function<void()>& func){ m_callback3 = func; }

private:
	float m_delay;
	float m_maxPercent;
	void initBG();
	cocos2d::Sprite* m_bg;
	cocos2d::Sprite* m_bar;

	std::function<void()> m_callback1;
	std::function<void()> m_callback2;
	std::function<void()> m_callback3;
};



#endif