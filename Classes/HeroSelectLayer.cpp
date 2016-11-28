#include "HeroSelectLayer.h"
#include "LoadingScene.h"
#include "LevelSelectedScene.h"
USING_NS_CC;

HeroSelect* HeroSelect::create()
{
	auto hs = new HeroSelect();
	if (hs && hs->init())
	{
		hs->autorelease();
		return hs;
	}
	CC_SAFE_DELETE(hs);
	return nullptr;
}

bool HeroSelect::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto size = Director::getInstance()->getVisibleSize();
	initBackGround();
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
				//callback
				auto scene = Scene::create();
				auto load = LoadingScene::create();
				load->bindNextSceneCallBack(LevelSelectedScene::createScene);
				scene->addChild(load);
				Director::getInstance()->replaceScene(TransitionMoveInL::create(0.5f, scene));
			}
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_returnButton);
		this->addChild(m_returnButton);
	}
	initTitle();
	initItemBG();
	initStartButton();

	initHeroIcon();
	initTeamList();

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

void HeroSelect::initHeroIcon()
{
	auto size = Director::getInstance()->getVisibleSize();
	//data
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
		if (i == 0)
		{
			state = "normal";
		}
		else
		{
			state = "disable";
		}
		auto icon = Sprite::create(StringUtils::format("gamescene/hero/%s_%s.png", m_firstPageStr[i].c_str(), state.c_str()));
		icon->setTag(1);
		node->addChild(icon);
		auto eff = Sprite::create("gamescene/selected_effect.png");
		eff->setTag(2);
		eff->setVisible(false);
		node->addChild(eff);
		auto x = i % 4;
		auto y = i / 4;
		auto iconSize = icon->getContentSize();
		node->setPosition(iconSize.width / 2 + 5 + x * (iconSize.width + 10), 0 - (iconSize.height / 2 + 5 + y * (iconSize.height + 10)));
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
		auto icon = Sprite::create(StringUtils::format("gamescene/hero/%s_%s.png", m_secondPageStr[i - 8].c_str(), state.c_str()));
		icon->setTag(1);
		node->addChild(icon);
		auto eff = Sprite::create("gamescene/selected_effect.png");
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
			auto lock = Sprite::create("gamescene/hero/lock.png");
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
	lis->onTouchEnded = [this](Touch*t, Event* e)->void{
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
				if (iconRect.containsPoint(newPos))
				{
					if (this->m_focus != nullptr)
					{
						m_focus->getChildByTag(2)->setVisible(false);
					}
					m_focus = p1;
					m_focus->getChildByTag(2)->setVisible(true);
					//list add
					m_teamList->addHero(m_focus->getTag());
					setStartButtonState(m_teamList->isFull());
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
				if (iconRect.containsPoint(newPos))
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
					//list add
					m_teamList->addHero(m_focus->getTag());
					setStartButtonState(m_teamList->isFull());
					break;
				}
			}
		}


	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_clipping);
	//init left and right button
	m_leftButton = Sprite::create("gamescene/button_left_normal.png");
	m_leftButton->setPosition(m_clipping->getPositionX() - m_leftButton->getContentSize().width / 2, m_clipping->getPositionY() + (122 * 2 + 20) / 2);
	this->addChild(m_leftButton);
	m_rightButton = Sprite::create("gamescene/button_right_normal.png");
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
		m_leftButton->setTexture("gamescene/button_left_normal.png");
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
		m_rightButton->setTexture("gamescene/button_right_normal.png");
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(rightLis, m_rightButton);
}

void HeroSelect::update(float dt)
{
	bgUpdate(dt);
}

void HeroSelect::initItemBG()
{
	auto size = Director::getInstance()->getVisibleSize();
	m_itemSelect = Node::create();
	//bg
	auto bg = Sprite::create("gamescene/skill_preview.png");
	bg->setTag(1);
	m_itemSelect->addChild(bg);
	//star
	m_star = MapLevelStar::create();
	m_star->setPosition(0, bg->getContentSize().height / 2);
	m_star->setStarNum(3);
	m_itemSelect->addChild(m_star);
	//items

	//preview button
	m_previewButton = Sprite::create("gamescene/preview.png");
	m_previewButton->setPosition(bg->getContentSize().width / 4, bg->getContentSize().height / 4);
	m_previewButton->setTag(2);
	m_previewButton->runAction(RepeatForever::create(
		Sequence::create(MoveBy::create(0.5f, Vec2(-100, 0)), 
						MoveBy::create(1.0f, Vec2(200, 0)), 
						MoveBy::create(0.5f, Vec2(-100, 0)), 
						nullptr)
	));
	m_itemSelect->addChild(m_previewButton);
	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [this](Touch* t, Event* e)->bool{
		auto pos = m_previewButton->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, m_previewButton->getContentSize() };
		if (rc.containsPoint(pos))
		{
			return true;
		}
		return false;
	};
	lis->onTouchEnded = [this](Touch* t, Event* e)->void{
		auto pos = m_previewButton->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, m_previewButton->getContentSize() };
		if (rc.containsPoint(pos))
		{
			this->runAction(MoveBy::create(0.25f, Vec2(0, 720)));
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_previewButton);

	m_itemSelect->setPosition(size.width / 5 * 4, size.height / 2);
	this->addChild(m_itemSelect);
}

void HeroSelect::initStartButton()
{
	m_startButton = Node::create();
	//bg
	auto bg = Sprite::create("gamescene/cant_start.png");
	bg->setTag(1);
	m_startButton->addChild(bg);
	//label

	m_startButton->setPosition(m_itemSelect->getPositionX(),
		m_itemSelect->getPositionY() - static_cast<Sprite*>(m_itemSelect->getChildByTag(1))->getContentSize().height / 2 - bg->getContentSize().height);
	this->addChild(m_startButton);
}

void HeroSelect::initTeamList()
{
	auto size = Director::getInstance()->getVisibleSize();
	m_teamList = TeamList::create();
	m_teamList->setPosition(m_clipping->getPositionX() + 100, size.height / 6);
	this->addChild(m_teamList);
	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [this](Touch* t, Event* e)->bool{
		auto pos = m_teamList->convertTouchToNodeSpace(t);
		auto rc = m_teamList->getBox();
		if (rc.containsPoint(pos))
		{
			return true;
		}
		return false;
	};
	lis->onTouchEnded = [this](Touch* t, Event* e)->void{
		auto pos = m_teamList->convertTouchToNodeSpace(t);
		auto rc = m_teamList->getBox();
		if (rc.containsPoint(pos))
		{
			m_teamList->getTouched(t);
			setStartButtonState(m_teamList->isFull());
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_teamList);
}

void HeroSelect::setStartButtonState(bool can)
{
	if (can)
	{
		m_startButton->getChildByTag(1)->removeFromParent();
		//bg
		auto bg = Sprite::create("gamescene/start_normal.png");
		bg->setTag(1);
		m_startButton->addChild(bg);
		auto lis = EventListenerTouchOneByOne::create();
		lis->setSwallowTouches(true);
		lis->onTouchBegan = [this, bg](Touch* t, Event* e)->bool{
			auto pos = bg->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, bg->getContentSize() };
			if (rc.containsPoint(pos))
			{
				bg->setTexture("gamescene/start_selected.png");
				return true;
			}
			return false;
		};
		lis->onTouchMoved = [this, bg](Touch* t, Event* e)->void{
			auto pos = bg->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, bg->getContentSize() };
			if (!rc.containsPoint(pos))
			{
				bg->setTexture("gamescene/start_normal.png");
			}
			else
			{
				bg->setTexture("gamescene/start_selected.png");
			}
		};
		lis->onTouchEnded = [this, bg](Touch* t, Event* e)->void{
			auto pos = bg->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, bg->getContentSize() };
			if (rc.containsPoint(pos))
			{
				bg->setTexture("gamescene/start_normal.png");
				//start callback
				CCLOG("start callback");
				this->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, 720)), CallFunc::create([this](){this->m_startcallback(); }), nullptr));
			}
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, bg);
		//label
		auto label = Sprite::create("gamescene/start_label.png");
		label->setPosition(bg->getContentSize() / 2);
		bg->addChild(label);
	}
	else
	{
		m_startButton->getChildByTag(1)->removeFromParent();
		//bg
		auto bg = Sprite::create("gamescene/cant_start.png");
		bg->setTag(1);
		m_startButton->addChild(bg);
		//label
	}
}

void HeroSelect::initTitle()
{
	auto size = Director::getInstance()->getVisibleSize();
	m_title = Node::create();
	//bg
	auto bg = Sprite::create("gamescene/title.png");
	bg->setTag(1);
	m_title->addChild(bg);

	//level label
	auto level = Sprite::create("gamescene/level_label.png");
	level->setPosition(0 - bg->getContentSize().width / 10, 0);
	level->setTag(2);
	m_title->addChild(level);
	//number label

	m_title->setPosition(size.width / 2, size.height - bg->getContentSize().height);
	this->addChild(m_title);
}

void HeroSelect::initBackGround()
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

void HeroSelect::bgUpdate(float dt)
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