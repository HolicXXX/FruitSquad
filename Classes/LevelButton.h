#ifndef _LEVEL_BUTTON_H_
#define _LEVEL_BUTTON_H_

#include "cocos2d.h"
#include "LevelStar.h"

class LevelButton : public cocos2d::Node
{
public:
	static LevelButton* create(int level);
	virtual bool init(int level);
	virtual void update(float dt);

	cocos2d::Size getButtonSize();
	void pass(int starNum);
	void setNormal();
	void setSelected();
	int getLevel(){ return m_level; }
private:
	int m_level;
	cocos2d::Label* m_levelNum;
	cocos2d::Sprite* m_button;
	LevelStar* m_star;
};


#endif