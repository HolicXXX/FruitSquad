#include "LevelStar.h"
USING_NS_CC;

LevelStar* LevelStar::create()
{
	auto star = new LevelStar();
	if (star && star->init())
	{
		star->autorelease();
		return star;
	}
	CC_SAFE_DELETE(star);
	return nullptr;
}

bool LevelStar::init()
{
	if (!Node::init())
	{
		return false;
	}
	m_starBG = nullptr;
	setStarNum();
	return true;
}

void LevelStar::setStarNum(int num)
{
	if (m_starBG == nullptr)
	{
		m_starBG = Sprite::create("levelselectscene/stars_bg.png");
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
		auto star = Sprite::create("levelselectscene/star_normal.png");
		star->setPosition(Vec2((i - 1) * 30, 0 + (i == 1 ? -2 : 4)));
		star->setRotation((i - 1) * 25);
		this->addChild(star);
	}
}
