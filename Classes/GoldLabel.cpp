#include "GoldLabel.h"
#include "DataManager.h"
USING_NS_CC;

GoldLabel* GoldLabel::create()
{
	auto gl = new GoldLabel();
	if (gl && gl->init())
	{
		gl->autorelease();
		return gl;
	}
	CC_SAFE_DELETE(gl);
	return nullptr;
}

bool GoldLabel::init()
{
	if (!Node::init())
	{
		return false;
	}
	this->setCascadeOpacityEnabled(true);
	m_label = nullptr;
	m_num = DataManager::getInstance()->getGoldNum();
	DataManager::getInstance()->registAddGoldCallBack(CC_CALLBACK_1(GoldLabel::addGold, this));
	DataManager::getInstance()->registCostGoldCallBack(CC_CALLBACK_1(GoldLabel::costGold, this));

	initBG();
	resetLabel();
	return true;
}

void GoldLabel::getCoin(int c)
{
	auto str = m_label->getString() + StringUtils::format("+%d", c).c_str();
	m_label->setString(str);
	this->runAction(Sequence::create(
		DelayTime::create(0.5f),
		CallFunc::create(
		[c]()->void{
		DataManager::getInstance()->addGold(c);
	}
	), nullptr));
}

void GoldLabel::addGold(int g)
{
	m_num += g;
	resetLabel();
}

void GoldLabel::costGold(int g)
{
	m_num -= g;
	resetLabel();
}

void GoldLabel::initBG()
{
	m_bg = Sprite::create("gamescene/ui/gold_bg.png");
	m_bg->setTag(1);
	this->addChild(m_bg);
}

void GoldLabel::resetLabel()
{
	if (!m_label)
	{
		//m_label = Label::createWithBMFont("fonts/ui_mid.fnt", StringUtils::format("%d", m_num));
		m_label = Label::createWithTTF(StringUtils::format("%d", m_num), "fonts/arial.ttf", 24);
		m_label->setTag(2);
		this->addChild(m_label);
		return;
	}
	m_label->setString(StringUtils::format("%d", m_num));
}