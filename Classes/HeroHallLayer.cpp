#include "HeroHallLayer.h"
#include "JsonTool.h"
USING_NS_CC;

HeroHall* HeroHall::create()
{
	auto hh = new HeroHall();
	if (hh && hh->init())
	{
		hh->autorelease();
		return hh;
	}
	CC_SAFE_DELETE(hh);
	return nullptr;
}

bool HeroHall::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto size = Director::getInstance()->getVisibleSize();
	m_focus = nullptr;
	m_property = nullptr;
	this->setPosition(Vec2(0, size.height));

	initBackGround();
	//title
	auto title = Node::create();
	{
		auto bg = Sprite::create("levelselectscene/herohall/title.png");
		bg->setTag(1);
		title->addChild(bg);
		auto label = Sprite::create("levelselectscene/herohall/text_title.png");
		label->setTag(2);
		title->addChild(label);
	}
	title->setPosition(size.width / 3, size.height - title->getChildByTag(1)->getContentSize().height);
	this->addChild(title);
	//return button
	{
		m_returnButton = Sprite::create("levelselectscene/herohall/button_return_normal.png");
		m_returnButton->setPosition(m_returnButton->getContentSize().width, size.height - m_returnButton->getContentSize().height);
		auto lis = EventListenerTouchOneByOne::create();
		lis->setSwallowTouches(true);
		lis->onTouchBegan = [this](Touch* t, Event* e)->bool{
			auto pos = m_returnButton->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, m_returnButton->getContentSize() };
			if (rc.containsPoint(pos))
			{
				m_returnButton->setTexture("levelselectscene/herohall/button_return_selected.png");
				return true;
			}
			return false;
		};
		lis->onTouchMoved = [this](Touch* t, Event* e)->void{
			auto pos = m_returnButton->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, m_returnButton->getContentSize() };
			if (rc.containsPoint(pos))
			{
				m_returnButton->setTexture("levelselectscene/herohall/button_return_selected.png");
			}
			else
			{
				m_returnButton->setTexture("levelselectscene/herohall/button_return_normal.png");
			}
		};
		lis->onTouchEnded = [this, size](Touch* t, Event* e)->void{
			auto pos = m_returnButton->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, m_returnButton->getContentSize() };
			if (rc.containsPoint(pos))
			{
				m_returnButton->setTexture("levelselectscene/herohall/button_return_normal.png");
				auto seq = Sequence::create(
					MoveBy::create(0.3f, Vec2(0, size.height)),
					nullptr);
				this->runAction(seq);
			}
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_returnButton);
		this->addChild(m_returnButton);
	}
	//init hero icon
	initHeroIcon();
	//model
	{
		m_heroModel = HeroModel::create(m_focus->getTag());
		m_heroModel->setPosition(size.width / 5 * 4, size.height / 2);
		this->addChild(m_heroModel);
	}

	initUI();
	initPropertyBox();

	//set swallow
	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [this](Touch* t, Event* e)->bool{
		auto pos = this->convertTouchToNodeSpace(t);
		auto rc = Rect{ 0, 0, 1024, 720 };
		if (rc.containsPoint(pos))
		{
			return true;
		}
		return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, this);
	this->scheduleUpdate();
	return true;
}

void HeroHall::onExit()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
	Node::onExit();
}

void HeroHall::initHeroIcon()
{
	auto size = Director::getInstance()->getVisibleSize();
	//data
	const int unlockhero = 3;
	std::vector<std::string> m_firstPageStr(8);
	m_firstPageStr[0] = "orange";
	m_firstPageStr[1] = "coconut";
	m_firstPageStr[2] = "apple";
	m_firstPageStr[3] = "pitaya";
	m_firstPageStr[4] = "guava";
	m_firstPageStr[5] = "watermelon";
	m_firstPageStr[6] = "pineapple";
	m_firstPageStr[7] = "pear";
	std::vector<std::string> m_secondPageStr(8);
	m_secondPageStr[0] = "cherry";
	m_secondPageStr[1] = "strawberry";
	m_secondPageStr[2] = "mangosteen";
	m_secondPageStr[3] = "evilorange";
	m_secondPageStr[4] = "evilapple";
	m_secondPageStr[5] = "unknow";
	m_secondPageStr[6] = "unknow";
	m_secondPageStr[7] = "unknow";
	//init clippingnode
	auto rectBox = DrawNode::create();
	rectBox->drawSolidRect(Vec2::ZERO, Vec2(122 * 4 + 40, 122 * 2 + 20), Color4F(1, 1, 1, 100));
	m_clipping = ClippingNode::create(rectBox);
	m_clipping->setPosition(size.width / 10, size.height / 5 * 2);
	this->addChild(m_clipping);

	m_firstPageOrigin = Node::create();
	m_firstPageOrigin->setPosition(Vec2(0, 122 * 2 + 20));
	m_clipping->addChild(m_firstPageOrigin);
	m_secondPageOrigin = Node::create();
	m_secondPageOrigin->setPosition(Vec2(122 * 4 + 40, 122 * 2 + 20));
	m_clipping->addChild(m_secondPageOrigin);

	for (int i = 0; i < 8; i++)
	{
		auto node = Node::create();
		//data
		auto state = std::string();
		if (i < unlockhero)
		{
			state = "normal";
		}
		else
		{
			state = "disable";
		}
		auto icon = Sprite::create(StringUtils::format("levelselectscene/herohall/hero/%s_%s.png", m_firstPageStr[i].c_str(),state.c_str()));
		icon->setTag(1);
		node->addChild(icon);
		auto eff = Sprite::create("levelselectscene/herohall/selected_effect.png");
		eff->setTag(2);
		eff->setVisible(false);
		node->addChild(eff);
		auto x = i % 4;
		auto y = i / 4;
		auto iconSize = icon->getContentSize();
		node->setPosition(iconSize.width / 2 + 5 + x * (iconSize.width + 10), 0- (iconSize.height / 2 + 5 + y * (iconSize.height + 10)));
		node->setTag(i);
		m_firstPage.pushBack(node);
		m_firstPageOrigin->addChild(node);
	}
	//init focus
	m_focus = m_firstPage.at(0);
	for (int i = 8; i < 16; i++)
	{
		auto node = Node::create();
		//data
		auto state = std::string();
		if (i - 8 > 2)
		{
			state = "normal";
		}
		else
		{
			state = "disable";
		}
		auto icon = Sprite::create(StringUtils::format("levelselectscene/herohall/hero/%s_%s.png", m_secondPageStr[i - 8].c_str(), state.c_str()));
		icon->setTag(1);
		node->addChild(icon);
		auto eff = Sprite::create("levelselectscene/herohall/selected_effect.png");
		eff->setTag(2);
		eff->setVisible(false);
		node->addChild(eff);
		auto x = (i - 8) % 4;
		auto y = (i - 8) / 4;
		auto iconSize = icon->getContentSize();
		node->setPosition(iconSize.width / 2 + 5 + x * (iconSize.width + 10), 0 - (iconSize.height / 2 + 5 + y * (iconSize.height + 10)));
		node->setTag(i);
		if (i - 8 == 3 || i - 8 == 4)
		{
			auto lock = Sprite::create("levelselectscene/herohall/hero/lock.png");
			lock->setTag(3);
			lock->setPosition(icon->getContentSize().width / 2 - lock->getContentSize().width / 2,
				0 - icon->getContentSize().height / 2 + lock->getContentSize().height / 2);
			node->addChild(lock);
		}
		m_secondPage.pushBack(node);
		m_secondPageOrigin->addChild(node);
	}
	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [this](Touch* t, Event* e)->bool{
		auto pos = m_clipping->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2(0, 0), Size(122 * 4 + 40, 122 * 2 + 20) };
		if (rc.containsPoint(pos))
		{
			return true;
		}
		return false;
	};
	lis->onTouchMoved = [this](Touch*t, Event* e)->void{
		auto newPos1 = m_firstPageOrigin->getPositionX() + t->getDelta().x;
		auto newPos2 = m_secondPageOrigin->getPositionX() + t->getDelta().x;
		if (newPos1 >= 0)
		{
			newPos1 = 0;
			newPos2 = 122 * 4 + 40;
		}
		else if (newPos2 <= 0)
		{
			newPos1 = -(122 * 4 + 40);
			newPos2 = 0;
		}
		m_firstPageOrigin->setPositionX(newPos1);
		m_secondPageOrigin->setPositionX(newPos2);
	};
	lis->onTouchEnded = [this, unlockhero](Touch*t, Event* e)->void{
		auto pos = m_clipping->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, Size(122 * 4 + 40, 122 * 2 + 20) };
		auto dis = t->getLocation() - t->getStartLocation();
		if (!rc.containsPoint(pos) || dis.length() >= 5)
		{
			if (dis.x > 0)
			{
				if (m_firstPageOrigin->getPositionX() < 0)
				{
					m_firstPageOrigin->runAction(MoveTo::create(0.25f, Vec2(0, m_firstPageOrigin->getPositionY())));
					m_secondPageOrigin->runAction(MoveTo::create(0.25f, Vec2(122 * 4 + 40, m_secondPageOrigin->getPositionY())));
				}
			}
			else if (dis.x < 0)
			{
				if (m_secondPageOrigin->getPositionX() > 0)
				{
					m_firstPageOrigin->runAction(MoveTo::create(0.25f, Vec2(-(122 * 4 + 40), m_firstPageOrigin->getPositionY())));
					m_secondPageOrigin->runAction(MoveTo::create(0.25f, Vec2(0, m_secondPageOrigin->getPositionY())));
				}
			}
			return;
		}
		if (m_firstPageOrigin->getPositionX() >= 0)
		{
			for (auto p1 : m_firstPage)
			{
				auto newPos = p1->convertTouchToNodeSpace(t);
				auto iconSize = p1->getChildByTag(1)->getContentSize();
				auto iconRect = Rect{ Vec2(-iconSize.width / 2, -iconSize.height / 2), iconSize };
				if (iconRect.containsPoint(newPos) && p1->getTag() < unlockhero)
				{
					if (this->m_focus != nullptr)
					{
						m_focus->getChildByTag(2)->setVisible(false);
					}
					m_focus = p1;
					m_focus->getChildByTag(2)->setVisible(true);
					//callback
					CCLOG("hero icon callback");
					m_heroModel->resetHeroIndex(p1->getTag());
					m_property->resetBox(p1->getTag());
					break;
				}
			}
		}
		else
		{
			for (auto p2 : m_secondPage)
			{
				auto newPos = p2->convertTouchToNodeSpace(t);
				auto iconSize = p2->getChildByTag(1)->getContentSize();
				auto iconRect = Rect{ Vec2(-iconSize.width / 2, -iconSize.height / 2), iconSize };
				if (iconRect.containsPoint(newPos) && p2->getTag() < unlockhero)
				{
					if (p2->getTag() > 12)
					{
						return;
					}
					if (this->m_focus != nullptr)
					{
						m_focus->getChildByTag(2)->setVisible(false);
					}
					m_focus = p2;
					m_focus->getChildByTag(2)->setVisible(true);
					//callback
					CCLOG("hero icon callback");
					m_heroModel->resetHeroIndex(p2->getTag());
					m_property->resetBox(p2->getTag());
					break;
				}
			}
		}
		
		
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_clipping);
	//init left and right button
	m_leftButton = Sprite::create("levelselectscene/herohall/button_left_normal.png");
	m_leftButton->setPosition(m_clipping->getPositionX() - m_leftButton->getContentSize().width / 2, m_clipping->getPositionY() + (122 * 2 + 20) / 2);
	this->addChild(m_leftButton);
	m_rightButton = Sprite::create("levelselectscene/herohall/button_right_normal.png");
	m_rightButton->setPosition(m_clipping->getPositionX() + 122 * 4 + 40 + m_rightButton->getContentSize().width / 2, m_clipping->getPositionY() + (122 * 2 + 20) / 2);
	this->addChild(m_rightButton);
	//button listener
	auto leftLis = EventListenerTouchOneByOne::create();
	leftLis->setSwallowTouches(true);
	leftLis->onTouchBegan = [this](Touch* t, Event* e)->bool{
		auto pos = m_leftButton->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, m_leftButton->getContentSize() };
		if (rc.containsPoint(pos))
		{
			m_leftButton->setTexture("levelselectscene/herohall/button_left_selected.png");
			return true;
		}
		return false;
	};
	leftLis->onTouchEnded = [this](Touch* t, Event* e)->void{
		auto pos = m_leftButton->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, m_leftButton->getContentSize() };
		if (rc.containsPoint(pos))
		{
			if (m_firstPageOrigin->getPositionX() < 0)
			{
				m_firstPageOrigin->runAction(MoveBy::create(0.25f, Vec2(122 * 4 + 40, 0)));
				m_secondPageOrigin->runAction(MoveBy::create(0.25f, Vec2(122 * 4 + 40, 0)));
			}
		}
		m_leftButton->setTexture("levelselectscene/herohall/button_left_normal.png");
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(leftLis, m_leftButton);
	auto rightLis = EventListenerTouchOneByOne::create();
	rightLis->setSwallowTouches(true);
	rightLis->onTouchBegan = [this](Touch* t, Event* e)->bool{
		auto pos = m_rightButton->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, m_rightButton->getContentSize() };
		if (rc.containsPoint(pos))
		{
			m_rightButton->setTexture("levelselectscene/herohall/button_right_selected.png");
			return true;
		}
		return false;
	};
	rightLis->onTouchEnded = [this](Touch* t, Event* e)->void{
		auto pos = m_rightButton->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, m_rightButton->getContentSize() };
		if (rc.containsPoint(pos))
		{
			if (m_firstPageOrigin->getPositionX() >= 0)
			{
				m_firstPageOrigin->runAction(MoveBy::create(0.25f, Vec2(-(122 * 4 + 40), 0)));
				m_secondPageOrigin->runAction(MoveBy::create(0.25f, Vec2(-(122 * 4 + 40), 0)));
			}
		}
		m_rightButton->setTexture("levelselectscene/herohall/button_right_normal.png");
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(rightLis, m_rightButton);
}

void HeroHall::update(float dt)
{
	bgUpdate(dt);
}

void HeroHall::initUI()
{
	auto size = Director::getInstance()->getVisibleSize();
	//init gems
	m_gems = Node::create();
	{
		auto bg = Sprite::create("levelselectscene/herohall/gems_number_bg.png");
		bg->setTag(1);
		m_gems->addChild(bg);
		//add label
		auto gemsNum = JsonTool::getInstance()->getDoc()["gems"].GetInt();
		auto label = Label::createWithBMFont("fonts/ui_mid.fnt", StringUtils::format("%d", gemsNum));
		label->setTag(2);
		m_gems->addChild(label);
		auto add = Sprite::create("levelselectscene/herohall/button_add_normal.png");
		add->setTag(3);
		add->setPosition(bg->getContentSize().width / 2 - 25, 0);
		auto lis = EventListenerTouchOneByOne::create();
		lis->setSwallowTouches(true);
		lis->onTouchBegan = [this, add](Touch* t, Event* e)->bool{
			auto pos = add->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, add->getContentSize() };
			if (rc.containsPoint(pos))
			{
				add->setTexture("levelselectscene/herohall/button_add_selected.png");
				return true;
			}
			return false;
		};
		lis->onTouchEnded = [this, add](Touch* t, Event* e)->void{
			auto pos = add->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, add->getContentSize() };
			if (rc.containsPoint(pos))
			{
				add->setTexture("levelselectscene/herohall/button_add_normal.png");
				//callback
				CCLOG("gems add callback");
			}
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, add);
		m_gems->addChild(add);
	}
	m_gems->setPosition(size.width - m_gems->getChildByTag(1)->getContentSize().width / 2 - 50,
		size.height - m_gems->getChildByTag(1)->getContentSize().height);
	this->addChild(m_gems);
	//init fruits
	m_fruits = Node::create();
	{
		auto bg = Sprite::create("levelselectscene/herohall/fruit_number_bg.png");
		bg->setTag(1);
		m_fruits->addChild(bg);
		//add label
		auto fruitNum = JsonTool::getInstance()->getDoc()["fruit"].GetInt();
		auto label = Label::createWithBMFont("fonts/ui_mid.fnt", StringUtils::format("%d", fruitNum));
		label->setTag(2);
		m_fruits->addChild(label);
		auto add = Sprite::create("levelselectscene/herohall/button_add_normal.png");
		add->setTag(3);
		add->setPosition(bg->getContentSize().width / 2 - 25, 0);
		auto lis = EventListenerTouchOneByOne::create();
		lis->setSwallowTouches(true);
		lis->onTouchBegan = [this, add](Touch* t, Event* e)->bool{
			auto pos = add->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, add->getContentSize() };
			if (rc.containsPoint(pos))
			{
				add->setTexture("levelselectscene/herohall/button_add_selected.png");
				return true;
			}
			return false;
		};
		lis->onTouchEnded = [this, add](Touch* t, Event* e)->void{
			auto pos = add->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, add->getContentSize() };
			if (rc.containsPoint(pos))
			{
				add->setTexture("levelselectscene/herohall/button_add_normal.png");
				//callback
				CCLOG("fruits add callback");
			}
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, add);
		m_fruits->addChild(add);
	}
	m_fruits->setPosition(size.width - m_gems->getChildByTag(1)->getContentSize().width / 2 - 50 - m_fruits->getChildByTag(1)->getContentSize().width - 50,
		size.height - m_fruits->getChildByTag(1)->getContentSize().height);
	this->addChild(m_fruits);
	NotificationCenter::getInstance()->addObserver(this, SEL_CallFuncO(&HeroHall::fruitDown), "costFruit", nullptr);
}

void HeroHall::fruitDown(Ref* sender)
{
	auto fruitNum = JsonTool::getInstance()->getDoc()["fruit"].GetInt();
	auto label = static_cast<Label*>(m_fruits->getChildByTag(2));
	label->setString(StringUtils::format("%d", fruitNum));
}

void HeroHall::initPropertyBox()
{
	auto size = Director::getInstance()->getVisibleSize();
	m_property = PropertyBox::create();
	m_property->resetBox(m_focus->getTag());
	m_property->setPosition(m_clipping->getPositionX() + 122 * 2 + 20, size.height / 5);
	this->addChild(m_property);
	NotificationCenter::getInstance()->addObserver(this, SEL_CallFuncO(&HeroHall::resetCallBack), "resetBox", nullptr);

}

void HeroHall::resetCallBack(Ref*)
{
	m_property->resetBox(m_focus->getTag());
}

void HeroHall::initBackGround()
{
	//background
	auto background1 = Sprite::create("loadingscene/background.png");
	background1->setPosition(background1->getContentSize() / 2);
	this->addChild(background1);
	auto background2 = Sprite::create("loadingscene/background.png");
	background2->setPosition(background1->getContentSize().width + background2->getContentSize().width / 2, background2->getContentSize().height / 2);
	this->addChild(background2);
	//back cloud
	m_bcloud1 = Sprite::create("loadingscene/cloud_blue.png");
	m_bcloud1->setPosition(m_bcloud1->getContentSize() / 2);
	this->addChild(m_bcloud1);
	m_bcloud2 = Sprite::create("loadingscene/cloud_blue.png");
	m_bcloud2->setFlippedX(true);
	m_bcloud2->setPosition(m_bcloud1->getPosition() + Vec2(m_bcloud2->getContentSize().width, 0));
	this->addChild(m_bcloud2);
	m_bcloud3 = Sprite::create("loadingscene/cloud_blue.png");
	m_bcloud3->setPosition(m_bcloud2->getPosition() + Vec2(m_bcloud3->getContentSize().width, 0));
	this->addChild(m_bcloud3);
	m_bcloud4 = Sprite::create("loadingscene/cloud_blue.png");
	m_bcloud4->setFlippedX(true);
	m_bcloud4->setPosition(m_bcloud3->getPosition() + Vec2(m_bcloud4->getContentSize().width, 0));
	this->addChild(m_bcloud4);

	m_wcloud1 = Sprite::create("loadingscene/cloud_white.png");
	m_wcloud1->setPosition(m_wcloud1->getContentSize() / 2);
	this->addChild(m_wcloud1);
	m_wcloud2 = Sprite::create("loadingscene/cloud_white.png");
	m_wcloud2->setFlippedX(true);
	m_wcloud2->setPosition(m_wcloud1->getPosition() + Vec2(m_wcloud2->getContentSize().width, 0));
	this->addChild(m_wcloud2);
	m_wcloud3 = Sprite::create("loadingscene/cloud_white.png");
	m_wcloud3->setPosition(m_wcloud2->getPosition() + Vec2(m_wcloud3->getContentSize().width, 0));
	this->addChild(m_wcloud3);
	m_wcloud4 = Sprite::create("loadingscene/cloud_white.png");
	m_wcloud4->setFlippedX(true);
	m_wcloud4->setPosition(m_wcloud3->getPosition() + Vec2(m_wcloud4->getContentSize().width, 0));
	this->addChild(m_wcloud4);
}

void HeroHall::bgUpdate(float dt)
{
	m_bcloud1->setPositionX(m_bcloud1->getPositionX() - 5);
	m_bcloud2->setPositionX(m_bcloud2->getPositionX() - 5);
	m_bcloud3->setPositionX(m_bcloud3->getPositionX() - 5);
	m_bcloud4->setPositionX(m_bcloud4->getPositionX() - 5);

	m_wcloud1->setPositionX(m_wcloud1->getPositionX() - 10);
	m_wcloud2->setPositionX(m_wcloud2->getPositionX() - 10);
	m_wcloud3->setPositionX(m_wcloud3->getPositionX() - 10);
	m_wcloud4->setPositionX(m_wcloud4->getPositionX() - 10);
	if (m_bcloud1->getPositionX() <= -(m_bcloud1->getContentSize().width / 2))
	{
		m_bcloud1->setPositionX(m_bcloud4->getPositionX() + m_bcloud1->getContentSize().width);
	}
	else if (m_bcloud2->getPositionX() <= -(m_bcloud2->getContentSize().width / 2))
	{
		m_bcloud2->setPositionX(m_bcloud1->getPositionX() + m_bcloud2->getContentSize().width);
	}
	else if (m_bcloud3->getPositionX() <= -(m_bcloud3->getContentSize().width / 2))
	{
		m_bcloud3->setPositionX(m_bcloud2->getPositionX() + m_bcloud3->getContentSize().width);
	}
	else if (m_bcloud4->getPositionX() <= -(m_bcloud4->getContentSize().width / 2))
	{
		m_bcloud4->setPositionX(m_bcloud3->getPositionX() + m_bcloud4->getContentSize().width);
	}

	if (m_wcloud1->getPositionX() <= -(m_wcloud1->getContentSize().width / 2))
	{
		m_wcloud1->setPositionX(m_wcloud4->getPositionX() + m_wcloud1->getContentSize().width);
	}
	else if (m_wcloud2->getPositionX() <= -(m_wcloud2->getContentSize().width / 2))
	{
		m_wcloud2->setPositionX(m_wcloud1->getPositionX() + m_wcloud2->getContentSize().width);
	}
	else if (m_wcloud3->getPositionX() <= -(m_wcloud3->getContentSize().width / 2))
	{
		m_wcloud3->setPositionX(m_wcloud2->getPositionX() + m_wcloud3->getContentSize().width);
	}
	else if (m_wcloud4->getPositionX() <= -(m_wcloud4->getContentSize().width / 2))
	{
		m_wcloud4->setPositionX(m_wcloud3->getPositionX() + m_wcloud4->getContentSize().width);
	}
}