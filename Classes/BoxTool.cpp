#include "BoxTool.h"
USING_NS_CC;
static BoxTool* _boxtool = nullptr;

BoxTool::BoxTool()
{
}

BoxTool::~BoxTool()
{
}

BoxTool* BoxTool::getInstance()
{
	if (!_boxtool)
	{
		_boxtool = new BoxTool();
	}
	return _boxtool;
}

Rect BoxTool::getBox(const std::vector<Vec2> & p)
{
	Vec2 ori = { 0, 0 };
	ori.x = minX(p);
	ori.y = minY(p);
	Size size = { 0, 0 };
	size.width = maxX(p) - ori.x;
	size.height = maxY(p) - ori.y;
	return Rect{ ori, size };
}

float BoxTool::minX(const std::vector<cocos2d::Vec2> & points)
{
	float ret = points[0].x;
	for (auto p : points)
	{
		if (p.x < ret)
		{
			ret = p.x;
		}
	}
	return ret;
}

float BoxTool::minY(const std::vector<cocos2d::Vec2> & points)
{
	float ret = points[0].y;
	for (auto p : points)
	{
		if (p.y < ret)
		{
			ret = p.y;
		}
	}
	return ret;
}

float BoxTool::maxX(const std::vector<cocos2d::Vec2> & points)
{
	float ret = points[0].x;
	for (auto p : points)
	{
		if (p.x > ret)
		{
			ret = p.x;
		}
	}
	return ret;
}

float BoxTool::maxY(const std::vector<cocos2d::Vec2> & points)
{
	float ret = points[0].y;
	for (auto p : points)
	{
		if (p.y > ret)
		{
			ret = p.y;
		}
	}
	return ret;
}