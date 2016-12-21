#include "MapLevelStar.h"
USING_NS_CC;
#include "DataManager.h"
#include "JsonTool.h"

MapLevelStar* MapLevelStar::create()
{
	auto star = new MapLevelStar();
	if (star && star->init())
	{
		star->autorelease();
		return star;
	}
	CC_SAFE_DELETE(star);
	return nullptr;
}

bool MapLevelStar::init()
{
	if (!Node::init())
	{
		return false;
	}
	m_starBG = nullptr;
	auto level = DataManager::getInstance()->getMapLevel();
	auto num = JsonTool::getInstance()->getDoc()["springlevels"][level - 1]["star"].GetInt();
	m_starNum = num;
	setStarNum(m_starNum);
	return true;
}

void MapLevelStar::setStarNum(int num)
{
	if (m_starBG == nullptr)
	{
		m_starBG = Sprite::create("gamescene/score_star_bg.png");
		this->addChild(m_starBG);
	}
	m_starNum = num;
	for (auto star : m_stars)
	{
		star->removeFromParent();
	}
	m_stars.clear();

	for (int i = 0; i < num; i++)
	{
		auto star = Sprite::create("gamescene/score_star.png");
		star->setPosition(Vec2((i - 1) * 90, 0 + (i == 1 ? 15 : -10)));
		star->setRotation((i - 1) * 20);
		this->addChild(star);
	}
}