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
	//base on level and data to create button and label
	{
		m_button = Sprite::create("levelselectscene/level_point_able_normal.png");
		this->addChild(m_button);
		m_levelNum = Label::createWithBMFont("fonts/level_number.fnt", StringUtils::format("%02d", level));
		m_levelNum->setPosition(Vec2(-5, 0));
		this->addChild(m_levelNum);

		//auto lis = EventListenerTouchOneByOne::create();
		//lis->setSwallowTouches(true);
		//lis->onTouchBegan = [this](Touch* t, Event* e)->bool{
		//	auto pos = this->m_button->convertTouchToNodeSpace(t);
		//	Rect rc = { 0, 0, this->m_button->getContentSize().width, this->m_button->getContentSize().height };
		//	if (rc.containsPoint(pos))
		//	{
		//		this->m_button->setTexture("levelselectscene/level_point_able_selected.png");
		//		return true;
		//	}
		//	else
		//		return false;
		//};
		//lis->onTouchMoved = [this](Touch* t, Event* e)->void{
		//	auto pos = this->m_button->convertTouchToNodeSpace(t);
		//	Rect rc = { 0, 0, this->m_button->getContentSize().width, this->m_button->getContentSize().height };
		//	if (!rc.containsPoint(pos))
		//	{
		//		this->m_button->setTexture("levelselectscene/level_point_able_normal.png");
		//	}
		//};
		//lis->onTouchEnded = [this](Touch* t, Event* e)->void{
		//	auto pos = this->m_button->convertTouchToNodeSpace(t);
		//	Rect rc = { 0, 0, this->m_button->getContentSize().width, this->m_button->getContentSize().height };
		//	if (rc.containsPoint(pos))
		//	{
		//		this->m_button->setTexture("levelselectscene/level_point_able_normal.png");
		//		//replace scene or callback
		//		CCLOG("callback");
		//		int num = int(CCRANDOM_0_1() * 3 + 1);
		//		this->pass(num);
		//	}
		//};
		//_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, this);
	}
	
	return true;
}

void LevelButton::update(float dt)
{

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
	m_button->setTexture("levelselectscene/level_point_able_normal.png");
}

void LevelButton::setSelected()
{
	//data
	m_button->setTexture("levelselectscene/level_point_able_selected.png");
}
void LevelButton::pass(int starNum)
{
	if (m_star != nullptr)
	{
		if (starNum == m_star->getStarNum())
			return;
		else
			m_star->removeFromParent();
	}
	m_star = LevelStar::create();
	m_star->setStarNum(starNum);
	m_star->setPosition(Vec2(0, -m_button->getContentSize().height / 2));
	this->addChild(m_star);
}
