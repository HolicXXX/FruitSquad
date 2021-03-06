#ifndef _BOXTOOL_H_
#define _BOXTOOL_H_

#include "cocos2d.h"

class BoxTool
{
public:
	static BoxTool* getInstance();

	cocos2d::Rect getBox(const std::vector<cocos2d::Vec2> & p);
	cocos2d::Rect getBox(cocos2d::Vec2 pos, float rotation,cocos2d::Size ori);
private:
	BoxTool();
	~BoxTool();

	float minX(const std::vector<cocos2d::Vec2> & points);
	float minY(const std::vector<cocos2d::Vec2> & points);
	float maxX(const std::vector<cocos2d::Vec2> & points);
	float maxY(const std::vector<cocos2d::Vec2> & points);
};



#endif