#include "SettlementStar.h"
USING_NS_CC;

SettlementStar* SettlementStar::create()
{
	auto ss = new SettlementStar();
	if (ss && ss->init())
	{
		ss->autorelease();
		return ss;
	}
	CC_SAFE_DELETE(ss);
	return nullptr;
}

bool SettlementStar::init()
{
	if (!Node::init())
	{
		return false;
	}
	m_starNum = 0;
	m_bg = Sprite::create("gamescene/settlement/settlement_star_bg.png");
	this->addChild(m_bg);

	return true;
}

void SettlementStar::addStar()
{
	if (m_starNum >= 3)
		return;
	m_starNum++;
	auto star = Sprite::create("gamescene/settlement/settlement_star.png");
	star->setPosition((m_starNum - 2) * 111, ((m_starNum == 2) ? (18) : (-6)));
	star->setRotation((m_starNum - 2) * 22);
	star->setScale(0.1f);
	star->runAction(Sequence::create(ScaleTo::create(0.3f, 1.1f), ScaleTo::create(0.05f, 1.0f), nullptr));
	this->addChild(star);
	m_stars.pushBack(star);
}