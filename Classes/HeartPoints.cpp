#include "HeartPoints.h"
USING_NS_CC;

HeartPoints* HeartPoints::create()
{
	auto hp = new HeartPoints();
	if (hp && hp->init())
	{
		hp->autorelease();
		return hp;
	}
	CC_SAFE_DELETE(hp);
	return nullptr;
}

bool HeartPoints::init()
{
	if (!Node::init())
	{
		return false;
	}
	auto size = Director::getInstance()->getVisibleSize();
	//init background
	m_hpBG = Sprite::create("levelselectscene/heart_bg.png");
	this->addChild(m_hpBG);
	//init hearts
	initHearts();

	this->scheduleUpdate();
	return true;
}

void HeartPoints::initHearts()
{
	//base on data
	for (int i = 0; i < 5; i++)
	{
		auto h = Sprite::create("levelselectscene/heart.png");
		h->setPosition(Vec2(-139 + i * 40, 0));
		m_hearts.pushBack(h);
		this->addChild(h);
	}
}

void HeartPoints::update(float dt)
{

}