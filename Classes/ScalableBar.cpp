#include "ScalableBar.h"
USING_NS_CC;

ScalableBar* ScalableBar::create(float maxPercent)
{
	auto sb = new ScalableBar();
	if (sb && sb->init(maxPercent))
	{
		sb->autorelease();
		return sb;
	}
	CC_SAFE_DELETE(sb);
	return nullptr;
}

bool ScalableBar::init(float maxPercent)
{
	if (!Node::create())
	{
		return false;
	}
	m_maxPercent = maxPercent;
	m_callback1 = nullptr;
	m_callback2 = nullptr;
	m_callback3 = nullptr;
	m_delay = 0;
	initBG();

	return true;
}

void ScalableBar::initBG()
{
	m_bg = Sprite::create("gamescene/settlement/goal_bar_bg.png");
	m_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	this->addChild(m_bg);
	m_bar = Sprite::create("gamescene/settlement/goal_bar_red.png");
	m_bar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_bar->setPosition(Vec2(2, 1));
	m_bar->setScaleX(0);
	this->addChild(m_bar);

	auto line1 = Sprite::create("gamescene/settlement/goal_line.png");
	auto pos = Vec2(m_bg->getContentSize().width / 9, m_bg->getContentSize().height / 2);
	line1->setPosition(pos.x * 3 + 2, 0);
	this->addChild(line1);
	auto sign1 = Sprite::create("gamescene/settlement/goal_num.png");
	sign1->setPosition(Vec2(line1->getPosition().x, sign1->getContentSize().height / 2 + line1->getContentSize().height / 2));
	this->addChild(sign1);
	auto timeslabel1 = Label::createWithTTF("5", "fonts/arial.ttf", 14);
	timeslabel1->setPosition(Vec2(28, 14));
	sign1->addChild(timeslabel1);
	auto star1 = Sprite::create("gamescene/settlement/goal_star_bg.png");
	star1->setName("star1");
	star1->setPosition(Vec2(line1->getPosition().x, -star1->getContentSize().height / 2 - line1->getContentSize().height / 2));
	this->addChild(star1);

	auto line2 = Sprite::create("gamescene/settlement/goal_line.png");
	line2->setPosition(pos.x * 6 + 2, 0);
	this->addChild(line2);
	auto sign2 = Sprite::create("gamescene/settlement/goal_num.png");
	sign2->setPosition(Vec2(line2->getPosition().x, sign2->getContentSize().height / 2 + line2->getContentSize().height / 2));
	this->addChild(sign2);
	auto timeslabel2 = Label::createWithTTF("2", "fonts/arial.ttf", 14);
	timeslabel2->setPosition(Vec2(28, 14));
	sign2->addChild(timeslabel2);
	auto star2 = Sprite::create("gamescene/settlement/goal_star_bg.png");
	star2->setName("star2");
	star2->setPosition(Vec2(line2->getPosition().x, -star2->getContentSize().height / 2 - line2->getContentSize().height / 2));
	this->addChild(star2);

	auto line3 = Sprite::create("gamescene/settlement/goal_line.png");
	line3->setPosition(pos.x * 8 + 2, 0);
	this->addChild(line3);
	auto sign3 = Sprite::create("gamescene/settlement/goal_num.png");
	sign3->setPosition(Vec2(line3->getPosition().x, sign3->getContentSize().height / 2 + line3->getContentSize().height / 2));
	this->addChild(sign3);
	auto timeslabel3 = Label::createWithTTF("0", "fonts/arial.ttf", 14);
	timeslabel3->setPosition(Vec2(28, 14));
	sign3->addChild(timeslabel3);
	auto star3 = Sprite::create("gamescene/settlement/goal_star_bg.png");
	star3->setName("star3");
	star3->setPosition(Vec2(line3->getPosition().x, -star3->getContentSize().height / 2 - line3->getContentSize().height / 2));
	this->addChild(star3);

	auto leftSign = Sprite::create("gamescene/settlement/goal_death.png");
	leftSign->setPosition(Vec2(-30, 0));
	this->addChild(leftSign);
	auto deathlabel = Label::createWithTTF("DEATH", "fonts/arial.ttf", 10);
	deathlabel->setPosition(Vec2(27, 11));
	leftSign->addChild(deathlabel);

}

void ScalableBar::update(float dt)
{
	m_delay += dt;
	if (m_delay >= m_maxPercent * 2.0f)
	{
		m_bar->setScaleX(1.0f);
		NotificationCenter::getInstance()->postNotification("settle");
		this->unscheduleUpdate();
	}
	m_bar->setScaleX(m_delay / 2.0f);
	if (m_delay >= 1.0f / 9.0f * 3 * 2.0f)
	{
		if (m_callback1)
		{
			static_cast<Sprite*>(this->getChildByName("star1"))->setTexture("gamescene/settlement/goal_star.png");
			m_callback1();
			m_bar->setTexture("gamescene/settlement/goal_bar_yellow.png");
			m_callback1 = nullptr;
		}
	}
	if (m_delay >= 1.0f / 9.0f * 6 * 2.0f)
	{
		if (m_callback2)
		{
			static_cast<Sprite*>(this->getChildByName("star2"))->setTexture("gamescene/settlement/goal_star.png");
			m_callback2();
			m_bar->setTexture("gamescene/settlement/goal_bar_blue.png");
			m_callback2 = nullptr;
		}
	}
	if (m_delay >= 1.0f / 9.0f * 8 * 2.0f)
	{
		if (m_callback3)
		{
			static_cast<Sprite*>(this->getChildByName("star3"))->setTexture("gamescene/settlement/goal_star.png");
			m_callback3();
			m_bar->setTexture("gamescene/settlement/goal_bar_green.png");
			m_callback3 = nullptr;
		}
	}

}
