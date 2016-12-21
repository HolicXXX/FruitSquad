#include "HeroModel.h"
#include "JsonTool.h"
#include "HeadFile.h"

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
			String str = JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["type"].GetString();
			auto label = Sprite::create(StringUtils::format("levelselectscene/herohall/hero_type_%s.png",str.getCString()));
			label->setTag(2);
			m_type->addChild(label);
		}
		m_type->setPosition(m_model->getContentSize() / 4);
		this->addChild(m_type);
		//label
		String name = JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["name"].GetString();
		m_nameText = Label::createWithBMFont("fonts/hero_name.fnt", name.getCString());
		m_nameText->setPosition(0, -170);
		this->addChild(m_nameText);
		return;
	}
	m_model->setTexture(StringUtils::format("levelselectscene/herohall/hero/%s_model.png", nameStrs[m_heroIndex].c_str()));
	auto label = static_cast<Sprite*>(m_type->getChildByTag(2));
	String str = JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["type"].GetString();
	label->setTexture(StringUtils::format("levelselectscene/herohall/hero_type_%s.png", str.getCString()));
	String name = JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["name"].GetString();
	m_nameText->setString(name.getCString());
}

void HeroModel::setButton()
{
	auto level = JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["level"].GetInt();
	//level reset button
	if (!m_levelResetButton)//first
	{
		m_levelResetButton = Node::create();
		{
			auto bg = Button::create("levelselectscene/herohall/button_resetlevel_normal.png", "levelselectscene/herohall/button_resetlevel_selected.png", "levelselectscene/herohall/button_zerolevel.png");
			bg->setTag(1);
			bg->setTitleFontSize(26);
			bg->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type)->void{
				auto level = JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["level"].GetInt();
				if (type == Widget::TouchEventType::ENDED)
				{
					auto fruitNum = JsonTool::getInstance()->getDoc()["fruit"].GetInt();
					JsonTool::getInstance()->getDoc()["fruit"].SetInt(fruitNum + JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["costfruit"].GetInt());
					JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["level"].SetInt(0);
					JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["costfruit"].SetInt(0);
					NotificationCenter::getInstance()->postNotification("costFruit");
					NotificationCenter::getInstance()->postNotification("resetBox");
					setButton();
					levelResetEff();
				}
			});
			m_levelResetButton->addChild(bg);
		}
		m_levelResetButton->setPosition(-(m_levelResetButton->getChildByTag(1)->getContentSize().width / 2), -240);
		this->addChild(m_levelResetButton);
	}
	auto bg = static_cast<Button*>(m_levelResetButton->getChildByTag(1));
	if (level == 0)
	{
		bg->setTitleText("Zero");
		bg->setTitleColor(Color3B::GRAY);
		bg->setEnabled(false);
	}
	else
	{
		bg->setTitleText("Reset");
		bg->setTitleColor(Color3B::GREEN);
		bg->setEnabled(true);
	}
	if (!m_levelUpButton)
	{
		m_levelUpButton = Node::create();
		{
			auto bg = Button::create("levelselectscene/herohall/button_up_normal.png", "levelselectscene/herohall/button_up_selected.png");
			bg->setTag(1);
			bg->setTitleFontSize(26);
			bg->setTitleColor(Color3B::ORANGE);
			bg->setTitleAlignment(TextHAlignment::CENTER);
			bg->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type)->void{
				if (type == Widget::TouchEventType::ENDED)
				{
					auto level = JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["level"].GetInt();
					auto fruitNum = JsonTool::getInstance()->getDoc()["fruit"].GetInt();
					if (fruitNum >= level + 1)
					{
						fruitNum -= (level + 1);
						JsonTool::getInstance()->getDoc()["fruit"].SetInt(fruitNum);
						JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["level"].SetInt(level + 1);
						JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["costfruit"].SetInt(JsonTool::getInstance()->getDoc()["heros"][m_heroIndex]["costfruit"].GetInt() + level + 1);
						NotificationCenter::getInstance()->postNotification("costFruit");
						NotificationCenter::getInstance()->postNotification("resetBox");
						setButton();
						levelUpEff();
					}
				}
			});
			m_levelUpButton->addChild(bg);
		}
		m_levelUpButton->setPosition(m_levelUpButton->getChildByTag(1)->getContentSize().width / 2, -240);
		this->addChild(m_levelUpButton);
	}
	auto upbg = static_cast<Button*>(m_levelUpButton->getChildByTag(1));
	upbg->setTitleText(StringUtils::format("%d    UP", level + 1));
}

void HeroModel::resetHeroIndex(int heroIndex)
{
	m_heroIndex = heroIndex;
	setModel();
	setButton();
}

void HeroModel::levelUpEff()
{
	auto label = Label::createWithTTF("LEVEL UP!", "fonts/arial.ttf", 48);
	label->setPosition(Vec2(0, -100));
	label->setTextColor(Color4B::YELLOW);
	label->setOpacity(0);
	this->addChild(label);
	label->runAction(Sequence::create(
		Spawn::create(FadeIn::create(0.1f), MoveBy::create(0.1f, Vec2(0, 100)), ScaleTo::create(0.1f, 1.5f), nullptr),
		DelayTime::create(0.1f),
		Spawn::create(FadeOut::create(0.1f), MoveBy::create(0.1f, Vec2(0, 100)), ScaleTo::create(0.1f, 1.0f), nullptr),
		CallFunc::create([label]()->void{label->removeFromParent(); }),
		nullptr));
}

void HeroModel::levelResetEff()
{
	auto label = Label::createWithTTF("RESET LEVEL!", "fonts/arial.ttf", 48);
	label->setPosition(Vec2(0, 100));
	label->setTextColor(Color4B::BLACK);
	label->setOpacity(0);
	this->addChild(label);
	label->runAction(Sequence::create(
		Spawn::create(FadeIn::create(0.1f), MoveBy::create(0.1f, Vec2(0, -100)), ScaleTo::create(0.1f, 1.5f), nullptr),
		DelayTime::create(0.1f),
		Spawn::create(FadeOut::create(0.1f), MoveBy::create(0.1f, Vec2(0, -100)), ScaleTo::create(0.1f, 1.0f), nullptr),
		CallFunc::create([label]()->void{label->removeFromParent(); }),
		nullptr));
}