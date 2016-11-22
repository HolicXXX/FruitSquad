#ifndef _LEVEL_STAR_H_
#define _LEVEL_STAR_H_

#include "cocos2d.h"

class LevelStar : public cocos2d::Node
{
public:
	static LevelStar* create();
	virtual bool init();

	void setStarNum(int num = 0);
	int getStarNum() { return m_starNum; }
private:
	int m_starNum;
	cocos2d::Sprite* m_starBG;
	cocos2d::Vector<cocos2d::Sprite*> m_stars;
};

#endif