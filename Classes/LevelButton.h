#ifndef _LEVEL_BUTTON_H_
#define _LEVEL_BUTTON_H_

#include "cocos2d.h"
#include "LevelStar.h"

enum PointState
{
	DISABLE,
	ABLE,
	PASS
};

class LevelButton : public cocos2d::Node
{
public:
	static LevelButton* create(int level);
	virtual bool init(int level);

	cocos2d::Size getButtonSize();

	void pass(int starNum);
	void setNormal();
	void setSelected();
	void setState(PointState state);
	PointState getState(){ return m_state; }
	int getLevel(){ return m_level; }
private:
	PointState m_state;
	std::vector<std::string> m_textureStr;
	int m_level;
	cocos2d::Label* m_levelNum;
	cocos2d::Sprite* m_button;
	LevelStar* m_star;
};


#endif