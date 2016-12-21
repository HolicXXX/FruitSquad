#include "PropertyBox.h"
#include "JsonTool.h"

bool PropertyBox::init()
{
	if (!Node::init())
	{
		return false;
	}
	m_bg = nullptr;
	m_hpBarBG = m_hpBar = nullptr;
	m_atkBarBG = m_atkBar = nullptr;
	m_skillBarBG = m_skillBar = nullptr;

	return true;
}

void PropertyBox::resetBox(int heroindex)
{
	auto level = JsonTool::getInstance()->getDoc()["heros"][heroindex]["level"].GetInt();
	auto hp = JsonTool::getInstance()->getDoc()["heros"][heroindex]["hp"].GetInt();
	auto hprate = JsonTool::getInstance()->getDoc()["heros"][heroindex]["hp_rate"].GetInt();
	auto att = JsonTool::getInstance()->getDoc()["heros"][heroindex]["attack"].GetInt();
	auto attrate = JsonTool::getInstance()->getDoc()["heros"][heroindex]["attack_rate"].GetInt();
	auto skill = JsonTool::getInstance()->getDoc()["heros"][heroindex]["skill"].GetInt();
	auto skillrate = JsonTool::getInstance()->getDoc()["heros"][heroindex]["skill_rate"].GetInt();
	if (!m_bg)
	{
		m_bg = Sprite::create("levelselectscene/herohall/property_box.png");
		this->addChild(m_bg);
	}
	//hp
	if (!m_hpBarBG)
	{
		m_hpBarBG = Scale9Sprite::create("levelselectscene/herohall/bar_hp_bg.png");
		m_hpBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		m_hpBarBG->setInsetLeft(10);
		m_hpBarBG->setInsetRight(136);
		m_hpBarBG->setPosition(Vec2(52, 112));
		m_bg->addChild(m_hpBarBG);

		m_hpBar = Scale9Sprite::create("levelselectscene/herohall/bar_hp.png");
		m_hpBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		m_hpBar->setInsetLeft(9);
		m_hpBar->setInsetRight(137);
		m_hpBar->setPosition(Vec2(52, 112));
		m_bg->addChild(m_hpBar);
	}
	m_hpBarBG->stopAllActions();
	m_hpBarBG->setScaleX(0);
	auto scaleto = (hp + level * hprate) / 500.0f;
	if (scaleto >= 1.0f)
		scaleto = 1.0f;
	m_hpBarBG->runAction(ScaleTo::create(0.25f, scaleto, 1.0f));
	m_hpBar->stopAllActions();
	m_hpBar->setScaleX(0);
	m_hpBar->runAction(ScaleTo::create(0.25f, hp / 500.0f,1.0f));
	//atk
	if (!m_atkBarBG)
	{
		m_atkBarBG = Scale9Sprite::create("levelselectscene/herohall/bar_atk_bg.png");
		m_atkBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		m_atkBarBG->setInsetLeft(10);
		m_atkBarBG->setInsetRight(136);
		m_atkBarBG->setPosition(Vec2(52, 72));
		m_bg->addChild(m_atkBarBG);

		m_atkBar = Scale9Sprite::create("levelselectscene/herohall/bar_atk.png");
		m_atkBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		m_atkBar->setInsetLeft(9);
		m_atkBar->setInsetRight(137);
		m_atkBar->setPosition(Vec2(52, 72));
		m_bg->addChild(m_atkBar);
	}
	m_atkBarBG->stopAllActions();
	m_atkBarBG->setScaleX(0);
	auto attscaleto = (att + level * attrate) / 100.0f;
	if (attscaleto >= 1.0f)
		attscaleto = 1.0f;
	m_atkBarBG->runAction(ScaleTo::create(0.25f, attscaleto, 1.0f));
	m_atkBar->stopAllActions();
	m_atkBar->setScaleX(0);
	m_atkBar->runAction(ScaleTo::create(0.25f, att / 100.0f, 1.0f));
	//skill
	if (!m_skillBarBG)
	{
		m_skillBarBG = Scale9Sprite::create("levelselectscene/herohall/bar_mana_bg.png");
		m_skillBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		m_skillBarBG->setInsetLeft(10);
		m_skillBarBG->setInsetRight(136);
		m_skillBarBG->setPosition(Vec2(52, 33));
		m_bg->addChild(m_skillBarBG);

		m_skillBar = Scale9Sprite::create("levelselectscene/herohall/bar_mana.png");
		m_skillBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		m_skillBar->setInsetLeft(9);
		m_skillBar->setInsetRight(137);
		m_skillBar->setPosition(Vec2(52, 33));
		m_bg->addChild(m_skillBar);
	}
	m_skillBarBG->stopAllActions();
	m_skillBar->setScaleX(0);
	auto skiscaleto = (skill + level * skillrate) / 1000.0f;
	if (skiscaleto >= 1.0f)
		skiscaleto = 1.0f;
	m_skillBarBG->runAction(ScaleTo::create(0.25f, skiscaleto, 1.0f));
	m_skillBar->stopAllActions();
	m_skillBar->setScaleX(0);
	m_skillBar->runAction(ScaleTo::create(0.25f, skill / 1000.0f, 1.0f));
}

void PropertyBox::update(float dt)
{

}

