#include "LevelButton.h"
USING_NS_CC;

LevelButton* LevelButton::create(int level)
{
	auto lb = new LevelButton();
	if (lb && lb->init(level))
	{
		lb->autorelease();
		return lb;
	}
	CC_SAFE_DELETE(lb);
	return nullptr;
}

bool LevelButton::init(int level)
{
	if (!Node::init())
	{
		return false;
	}
	m_level = level;
	m_star = nullptr;
	m_state = DISABLE;
	m_textureStr.push_back("disable");
	m_textureStr.push_back("able");
	m_textureStr.push_back("pass");
	//base on level and data to create button and label
	{
		m_button = Sprite::create("levelselectscene/level_point_able_normal.png");
		this->addChild(m_button);
		m_levelNum = Label::createWithBMFont("fonts/level_number.fnt", StringUtils::format("%02d", level));
		m_levelNum->setPosition(Vec2(-5, 0));
		this->addChild(m_levelNum);
	}
	
	return true;
}

Size LevelButton::getButtonSize()
{
	if (m_button)
		return m_button->getContentSize();
	return Size::ZERO;
}

void LevelButton::setNormal()
{
	//data
	auto str = m_textureStr[int(m_state)];
	m_button->setTexture(StringUtils::format("levelselectscene/level_point_%s_normal.png", str.c_str()));
}

void LevelButton::setSelected()
{
	//data
	auto str = m_textureStr[int(m_state)];
	m_button->setTexture(StringUtils::format("levelselectscene/level_point_%s_selected.png", str.c_str()));
}

void LevelButton::setState(ButtonState state)
{
	m_state = state;
	if (m_state == DISABLE)
	{
		m_button->setTexture("levelselectscene/level_point_disable.png");
	}
	else
	{
		setNormal();
	}
}

void LevelButton::pass(int starNum)
{
	if (m_star != nullptr)
	{
		if (starNum != m_star->getStarNum())
			m_star->setStarNum(starNum);
		return;
	}
	setState(ButtonState::PASS);
	m_star = LevelStar::create();
	m_star->setStarNum(starNum);
	m_star->setPosition(Vec2(0, -m_button->getContentSize().height / 2));
	this->addChild(m_star);
}
