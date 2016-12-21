#include "SettlementFruit.h"
USING_NS_CC;

SettlementFruit* SettlementFruit::create(bool isget, int num)
{
	auto sf = new SettlementFruit();
	if (sf && sf->init(isget,num))
	{
		sf->autorelease();
		return sf;
	}
	CC_SAFE_DELETE(sf);
	return nullptr;
}

bool SettlementFruit::init(bool isget, int num)
{
	if (!Node::init())
	{
		return false;
	}
	this->setCascadeOpacityEnabled(true);
	m_fruit = nullptr;
	m_mask = nullptr;
	m_isGet = isget;
	m_num = num;

	m_fruit = Sprite::create(StringUtils::format("gamescene/settlement/get_fruit_%02d.png",m_num));
	this->addChild(m_fruit);
	if (!m_isGet)
	{
		m_mask = Sprite::create("gamescene/settlement/get_fruit_00.png");
		this->addChild(m_mask);
		m_fruit->setVisible(false);
	}

	return true;
}

void SettlementFruit::getFruit()
{
	if (m_isGet)
		return;
	this->runAction(Sequence::create(
		ScaleTo::create(0.1f,1.2f),
		CallFunc::create([this](){
		m_mask->removeFromParent();
		m_fruit->setVisible(true);
		m_isGet = true;
	}),	
		ScaleTo::create(0.1f,1.0f),
		nullptr));
}