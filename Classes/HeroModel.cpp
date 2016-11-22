#include "HeroModel.h"
USING_NS_CC;

HeroModel* HeroModel::create(int heroIndex)
{
	auto hm = new HeroModel();
	if (hm && hm->init(heroIndex))
	{
		hm->autorelease();
		return hm;
	}
	CC_SAFE_DELETE(hm);
	return nullptr;
}

bool HeroModel::init(int heroIndex)
{
	if (!Node::init())
	{
		return false;
	}
	m_heroIndex = heroIndex;
	m_model = nullptr;
	m_lockButton = nullptr;
	m_levelResetButton = nullptr;
	m_levelUpButton = nullptr;
	auto size = Director::getInstance()->getVisibleSize();
	//init first
	setModel();
	setButton();
	return true;
}

void HeroModel::setModel()
{
	//data
	std::vector<std::string> nameStrs(13);
	nameStrs[0] = "orange";
	nameStrs[1] = "coconut";
	nameStrs[2] = "apple";
	nameStrs[3] = "pitaya";
	nameStrs[4] = "guava"; 
	nameStrs[5] = "watermelon";
	nameStrs[6] = "pineapple";
	nameStrs[7] = "pear";
	nameStrs[8] = "cherry";
	nameStrs[9] = "strawberry";
	nameStrs[10] = "mangosteen";
	nameStrs[11] = "evilorange";
	nameStrs[12] = "apple";
	if (!m_model)//first
	{
		//model
		m_model = Sprite::create(StringUtils::format("levelselectscene/herohall/hero/%s_model.png", nameStrs[m_heroIndex].c_str()));
		this->addChild(m_model);
		//type
		m_type = Node::create();
		{
			auto effBG = Sprite::create("levelselectscene/herohall/hero_type_effect.png");
			effBG->setTag(1);
			m_type->addChild(effBG);
			//data
			auto label = Sprite::create("levelselectscene/herohall/hero_type_attack.png");
			label->setTag(2);
			m_type->addChild(label);
		}
		m_type->setPosition(m_model->getContentSize() / 4);
		this->addChild(m_type);
		//label
		return;
	}
	m_model->setTexture(StringUtils::format("levelselectscene/herohall/hero/%s_model.png", nameStrs[m_heroIndex].c_str()));
}

void HeroModel::setButton()
{
	//data

	//level reset button
	if (!m_levelResetButton)//first
	{
		m_levelResetButton = Node::create();
		{
			auto bg = Sprite::create("levelselectscene/herohall/button_zerolevel.png");
			bg->setTag(1);
			m_levelResetButton->addChild(bg);
			//label
		}
		m_levelResetButton->setPosition(-(m_levelResetButton->getChildByTag(1)->getContentSize().width / 2), -200);
		this->addChild(m_levelResetButton);
	}
	if (!m_levelUpButton)
	{
		m_levelUpButton = Node::create();
		{
			auto bg = Sprite::create("levelselectscene/herohall/button_up_normal.png");
			bg->setTag(1);
			m_levelUpButton->addChild(bg);
			//label
		}
		m_levelUpButton->setPosition(m_levelUpButton->getChildByTag(1)->getContentSize().width / 2, -200);
		this->addChild(m_levelUpButton);
	}
}

void HeroModel::resetHeroIndex(int heroIndex)
{
	m_heroIndex = heroIndex;
	setModel();
	setButton();
}