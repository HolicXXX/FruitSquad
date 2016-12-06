#ifndef _HPBOTTLE_H_
#define _HPBOTTLE_H_

#include "cocos2d.h"

class HPBottle : public cocos2d::Node
{
public:
	static HPBottle* create();
	virtual bool init();
	virtual void update(float dt);

	void bindCallBack(const std::function<void()> & func){ m_callback = func; }
private:
	int m_num;
	std::string m_stateName;

	void initBG();
	cocos2d::Sprite* m_bg;
	//add label

	std::function<void()> m_callback;
	
	void initCD();
	float m_cdTime;
	bool m_isCD;
	cocos2d::Node* m_cd;
};

#endif