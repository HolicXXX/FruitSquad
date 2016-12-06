#include "orangeSkill.h"
#include "cocostudio/CocoStudio.h"
#include "BoxTool.h"
USING_NS_CC;
using namespace cocostudio;

orangeSkill* orangeSkill::create(SkillLevel l, cocos2d::Vec2 dir)
{
	auto os = new orangeSkill();
	if (os && os->init(l, dir))
	{
		os->autorelease();
		return os;
	}
	CC_SAFE_DELETE(os);
	return nullptr;
}

bool orangeSkill::init(SkillLevel l, cocos2d::Vec2 dir)
{
	if (!Node::init())
	{
		return false;
	}
	m_level = l;
	m_speed = 6;
	m_dir = dir;
	initAni();
	initDir();
	
	return true;
}

void orangeSkill::initAni()
{
	auto ani = Armature::create("orangeskill");
	ani->setName("animation");
	std::string str = "";
	switch (m_level)
	{
	case SKILL_LOW:
	{
		str = "skill_low";
	}
		break;
	case SKILL_MID:
	{
		str = "skill_mid";
	}
		break;
	case SKILL_HIGH:
	{
		str = "skill_high";
	}
		break;
	default:
		break;
	}
	ani->getAnimation()->play(str);
	this->addChild(ani);
}

void orangeSkill::initDir()
{
	auto ra = m_dir.getAngle();
	m_rotation = -CC_RADIANS_TO_DEGREES(ra);
	this->setRotation(m_rotation);
}

void orangeSkill::setBox()
{
	auto skillSize = Size(220, 250);
	std::vector<Vec2> points;
	auto p1 = this->convertToWorldSpace(Vec2(0 - 110, 0 - 125));//leftbottom
	points.push_back(p1);
	auto p2 = this->convertToWorldSpace(Vec2(0 + 110, 0 - 125));//rightbottom
	points.push_back(p2);
	auto p3 = this->convertToWorldSpace(Vec2(0 + 110, 0 + 125));//righttop
	points.push_back(p3);
	auto p4 = this->convertToWorldSpace(Vec2(0 - 110, 0 + 125));//lefttop
	points.push_back(p4);

	m_hitRect = BoxTool::getInstance()->getBox(points);//box on worldspace
}

void orangeSkill::update(float dt)
{
	this->setPosition(this->getPosition() + m_dir*m_speed);
	m_hitRect.origin += m_dir * m_speed;//box move
	CCLOG("%f,%f", m_hitRect.origin.x, m_hitRect.origin.y);
}