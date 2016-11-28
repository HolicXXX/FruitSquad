#include "LevelSelectedScene.h"
#include "StartScene.h"
#include "SeasonLayer.h"
#include "HeroHallLayer.h"
USING_NS_CC;

Scene* LevelSelectedScene::createScene()
{
	auto lselect = LevelSelectedScene::create();
	auto scene = Scene::create();
	scene->addChild(lselect);
	return scene;
}

LevelSelectedScene* LevelSelectedScene::create()
{
	auto lselect = new LevelSelectedScene();
	if (lselect && lselect->init())
	{
		lselect->autorelease();
		return lselect;
	}
	CC_SAFE_DELETE(lselect);
	return nullptr;
}

bool LevelSelectedScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto size = Director::getInstance()->getVisibleSize();
	m_focus = nullptr;
	//init background
	m_background = Sprite::create("levelselectscene/spring_bg.png");
	m_background->setPosition(size / 2);
	this->addChild(m_background);
	//init level button
	initLevelButton();
	//init ui
	initUI();

	scheduleOnce([this](float)->void{
		auto seasonlayer = SeasonLayer::create();
		//bind callback
		this->getScene()->addChild(seasonlayer);
	}, 0.5f, "seasonlayer");
	this->scheduleUpdate();

	m_heroHallLayer = HeroHall::create();
	this->addChild(m_heroHallLayer);

	return true;
}

void LevelSelectedScene::update(float dt)
{

}

void LevelSelectedScene::initLevelButton()
{
	auto size = Director::getInstance()->getVisibleSize();
	//init left and right button
	m_leftButton = Sprite::create("levelselectscene/button_left_normal.png");
	m_leftButton->setPosition(size.width / 6, size.height / 2);
	m_leftButton->setVisible(false);
	this->addChild(m_leftButton);

	m_rightButton = Sprite::create("levelselectscene/button_right_normal.png");
	m_rightButton->setPosition(size.width / 6 * 5, size.height / 2);
	this->addChild(m_rightButton);

	auto leftLis = EventListenerTouchOneByOne::create();
	leftLis->setEnabled(false);
	leftLis->setSwallowTouches(true);
	auto rightLis = EventListenerTouchOneByOne::create();
	rightLis->setSwallowTouches(true);
	//left
	{
		leftLis->onTouchBegan = [this](Touch* t, Event* e)->bool{
			auto pos = m_leftButton->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, m_leftButton->getContentSize() };
			if (rc.containsPoint(pos))
			{
				m_leftButton->setTexture("levelselectscene/button_left_selected.png");
				return true;
			}
			return false;
		};
		leftLis->onTouchMoved = [this](Touch* t, Event* e)->void{
			auto pos = m_leftButton->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, m_leftButton->getContentSize() };
			if (!rc.containsPoint(pos))
			{
				m_leftButton->setTexture("levelselectscene/button_left_normal.png");
			}
			else
			{
				m_leftButton->setTexture("levelselectscene/button_left_selected.png");
			}
		};
		leftLis->onTouchEnded = [this, leftLis, rightLis](Touch* t, Event* e)->void{
			auto pos = m_leftButton->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, m_leftButton->getContentSize() };
			if (rc.containsPoint(pos))
			{
				m_leftButton->setVisible(false);
				leftLis->setEnabled(false);
				m_rightButton->setVisible(true);
				rightLis->setEnabled(true);
				auto size = Director::getInstance()->getVisibleSize();
				m_firstMid->runAction(MoveBy::create(0.5f, Vec2(size.width / 5 * 3, 0)));
				m_secondMid->runAction(MoveBy::create(0.5f, Vec2(size.width / 5 * 3, 0)));
			}
			m_leftButton->setTexture("levelselectscene/button_left_normal.png");
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(leftLis, m_leftButton);
	}
	//right
	{
		rightLis->onTouchBegan = [this](Touch* t, Event* e)->bool{
			auto pos = m_rightButton->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, m_rightButton->getContentSize() };
			if (rc.containsPoint(pos))
			{
				m_rightButton->setTexture("levelselectscene/button_right_selected.png");
				return true;
			}
			return false;
		};
		rightLis->onTouchMoved = [this](Touch* t, Event* e)->void{
			auto pos = m_rightButton->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, m_rightButton->getContentSize() };
			if (!rc.containsPoint(pos))
			{
				m_rightButton->setTexture("levelselectscene/button_right_normal.png");
			}
			else
			{
				m_rightButton->setTexture("levelselectscene/button_right_selected.png");
			}
		};
		rightLis->onTouchEnded = [this, leftLis, rightLis](Touch* t, Event* e)->void{
			auto pos = m_rightButton->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, m_rightButton->getContentSize() };
			if (rc.containsPoint(pos))
			{
				m_rightButton->setVisible(false);
				rightLis->setEnabled(false);
				m_leftButton->setVisible(true);
				leftLis->setEnabled(true);
				auto size = Director::getInstance()->getVisibleSize();
				m_firstMid->runAction(MoveBy::create(0.5f, Vec2(-size.width / 5 * 3, 0)));
				m_secondMid->runAction(MoveBy::create(0.5f, Vec2(-size.width / 5 * 3, 0)));
			}
			m_rightButton->setTexture("levelselectscene/button_right_normal.png");
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(rightLis, m_leftButton);
	}

	auto rc = DrawNode::create();
	rc->drawSolidRect(Vec2(0, 0), Vec2(size.width / 5 * 3, size.height / 5 * 3), Color4F(255, 255, 255, 0));
	m_cliping = ClippingNode::create(rc);
	m_cliping->setPosition(size / 5);
	this->addChild(m_cliping);
	m_firstMid = Node::create();
	m_firstMid->setPosition(Vec2(size / 5 * 3) / 2);
	m_cliping->addChild(m_firstMid);
	m_secondMid = Node::create();
	m_secondMid->setPosition(Vec2(size / 5 * 3) / 2 + Vec2(size.width / 5 * 3, 0));
	m_cliping->addChild(m_secondMid);
	for (int i = 0; i < 15; i++)
	{
		auto button = LevelButton::create(i + 1);
		auto x = i % 5;
		auto y = i / 5;
		auto offsetX = (x >= 2) ? (x == 2 ? 0 : ((x - 2) * 120)) : ((2 - x) * -120);
		auto offsetY = (y > 1) ? (-120) : (((y == 1) ? 0 : 120));
		button->setPosition(Vec2(offsetX, offsetY));
		m_firstpage.pushBack(button);
		m_firstMid->addChild(button);
	}
	for (int i = 15; i < 30; i++)
	{
		auto button = LevelButton::create(i + 1);
		auto x = (i - 15) % 5;
		auto y = (i - 15) / 5;
		auto offsetX = (x >= 2) ? (x == 2 ? 0 : ((x - 2) * 120)) : ((2 - x) * -120);
		auto offsetY = (y > 1) ? (-120) : (((y == 1) ? 0 : 120));
		button->setPosition( Vec2(offsetX, offsetY));
		m_secondpage.pushBack(button);
		m_secondMid->addChild(button);
	}
	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [this,size](Touch* t, Event* e)->bool{
		auto pos = m_cliping->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, Size(size.width / 5 * 3, size.height / 5 * 3) };
		if (rc.containsPoint(pos))
		{
			if (m_firstMid > 0)
			{
				for (auto b1 : m_firstpage)
				{
					auto pos = b1->convertTouchToNodeSpace(t);
					auto buttonRc = Rect{ Vec2(-b1->getButtonSize().width / 2, -b1->getButtonSize().height / 2), b1->getButtonSize() };
					if (buttonRc.containsPoint(pos))
					{
						if (b1->getState() != ButtonState::DISABLE)
						{
							if (m_focus)
							{
								m_focus->setNormal();
							}
							b1->setSelected();
							m_focus = b1;
						}
						break;
					}
				}
			}
			else
			{
				for (auto b2 : m_secondpage)
				{
					auto pos = b2->convertTouchToNodeSpace(t);
					auto buttonRc = Rect{ Vec2(-b2->getButtonSize().width / 2, -b2->getButtonSize().height / 2), b2->getButtonSize() };
					if (buttonRc.containsPoint(pos))
					{
						if (b2->getState() != ButtonState::DISABLE)
						{
							if (m_focus)
							{
								m_focus->setNormal();
							}
							b2->setSelected();
							m_focus = b2;
						}
						break;
					}
				}
			}
			return true;
		}
		return false;
	};
	lis->onTouchMoved = [this, size](Touch* t, Event* e)->void{
		if (t->getDelta().length() > 0)
		{
			if (m_focus)
			{
				m_focus->setNormal();
			}
			auto newPos1 = m_firstMid->getPositionX() + t->getDelta().x;
			auto newPos2 = m_secondMid->getPositionX() + t->getDelta().x;
			if (newPos1 >= size.width / 10 * 3)
			{
				newPos1 = size.width / 10 * 3;
				newPos2 = size.width / 10 * 3 + size.width / 5 * 3;
			}
			else if (newPos2 <= size.width / 10 * 3)
			{
				newPos1 = -size.width / 10 * 3;
				newPos2 = size.width / 10 * 3;
			}
			m_firstMid->setPositionX(newPos1);
			m_secondMid->setPositionX(newPos2);
		}
	};
	lis->onTouchEnded = [this, size, leftLis, rightLis](Touch* t, Event* e)->void{
		auto pos = m_cliping->convertTouchToNodeSpace(t);
		auto rc = Rect{ Vec2::ZERO, Size(size.width / 5 * 3, size.height / 5 * 3) };
		auto dis = t->getLocation() - t->getStartLocation();
		if (!rc.containsPoint(pos) || dis.length() >= 5)
		{
			if (dis.x > 0)
			{
				if (m_firstMid->getPositionX() < size.width / 10 * 3)
				{
					auto move = Vec2(size.width / 10 * 3 - m_firstMid->getPositionX(), 0);
					m_firstMid->runAction(MoveBy::create(0.25f, move));
					m_secondMid->runAction(MoveBy::create(0.25f, move));
					m_leftButton->setVisible(false);
					leftLis->setEnabled(false);
					m_rightButton->setVisible(true);
					rightLis->setEnabled(true);
				}
			}
			else if (dis.x < 0)
			{
				if (m_firstMid->getPositionX() > -size.width / 10 * 3)
				{
					auto move = Vec2(-size.width / 10 * 3 - m_firstMid->getPositionX(), 0);
					m_firstMid->runAction(MoveBy::create(0.25f, move));
					m_secondMid->runAction(MoveBy::create(0.25f, move));
					m_leftButton->setVisible(true);
					leftLis->setEnabled(true);
					m_rightButton->setVisible(false);
					rightLis->setEnabled(false);
				}
			}
			return;
		}
		if (m_firstMid->getPositionX() > 0)
		{
			for (auto b1 : m_firstpage)
			{
				auto pos = b1->convertTouchToNodeSpace(t);
				auto buttonSize = b1->getButtonSize();
				auto buttonRc = Rect{ Vec2(-buttonSize.width / 2, -buttonSize.height / 2), buttonSize };
				if (buttonRc.containsPoint(pos))
				{
					if (b1->getState() != ButtonState::DISABLE)
					{
						b1->setNormal();
						m_focus = b1;
						//callback
					}
					break;
				}
			}
		}
		else
		{
			for (auto b2 : m_secondpage)
			{
				auto pos = b2->convertTouchToNodeSpace(t);
				auto buttonSize = b2->getButtonSize();
				auto buttonRc = Rect{ Vec2(-buttonSize.width / 2, -buttonSize.height / 2), buttonSize };
				if (buttonRc.containsPoint(pos))
				{
					if (b2->getState() != ButtonState::DISABLE)
					{
						b2->setNormal();
						m_focus = b2;
						//callback
					}
					break;
				}
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_cliping);
}

void LevelSelectedScene::initUI()
{
	auto size = Director::getInstance()->getVisibleSize();
	//home button
	m_home = MenuItemImage::create("levelselectscene/button_home_normal.png", 
		"levelselectscene/button_home_selected.png", 
		[](Ref*)->void{
		auto scene = StartScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
	});
	auto homeMenu = Menu::create(m_home, nullptr);
	homeMenu->setPosition(50, size.height - 50);
	this->addChild(homeMenu);

	//heartpoints
	m_heartPoints = HeartPoints::create();
	m_heartPoints->setPosition(size.width - 180, size.height - 40);
	this->addChild(m_heartPoints);
	//gems
	m_gems = Node::create();
	{
		auto bg = Sprite::create("levelselectscene/gems_bg.png");
		bg->setTag(1);
		m_gems->addChild(bg);
		//add label

		auto button = Sprite::create("levelselectscene/button_add_normal.png");
		button->setTag(3);
		button->setPosition(Vec2(80, 0));
		m_gems->addChild(button);
		auto buttonlis = EventListenerTouchOneByOne::create();
		buttonlis->setSwallowTouches(true);
		buttonlis->onTouchBegan = [this, button](Touch* t, Event * e)->bool{
			auto pos = button->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, button->getContentSize() };
			if (rc.containsPoint(pos))
			{
				button->setTexture("levelselectscene/button_add_selected.png");
				return true;
			}
			return false;
		};
		buttonlis->onTouchMoved = [this, button](Touch* t, Event * e)->void{
			auto pos = button->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, button->getContentSize() };
			if (!rc.containsPoint(pos))
			{
				button->setTexture("levelselectscene/button_add_normal.png");
			}
			else
			{
				button->setTexture("levelselectscene/button_add_selected.png");
			}
		};
		buttonlis->onTouchEnded = [this, button](Touch* t, Event * e)->void{
			auto pos = button->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, button->getContentSize() };
			if (rc.containsPoint(pos))
			{
				//callback
				CCLOG("add callback");
			}
			button->setTexture("levelselectscene/button_add_normal.png");
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(buttonlis, button);
	}
	m_gems->setPosition(550, size.height - 40);
	this->addChild(m_gems);
	//keys
	m_keys = Node::create();
	{
		auto bg = Sprite::create("levelselectscene/keys_bg.png");
		bg->setTag(1);
		m_keys->addChild(bg);
		//add label
	}
	m_keys->setPosition(350, size.height - 40);
	this->addChild(m_keys);

	//left down buttons
	m_seasons = MenuItemImage::create("levelselectscene/seasons_normal.png",
		"levelselectscene/seasons_selected.png",
		[this](Ref*)->void{
		auto seasonlayer = SeasonLayer::create();
		//bind callback
		this->addChild(seasonlayer);
	});
	m_heroHall = MenuItemImage::create("levelselectscene/herohall_normal.png",
		"levelselectscene/herohall_selected.png",
		[this](Ref*)->void{
		auto size = Director::getInstance()->getVisibleSize();
		m_heroHallLayer->runAction(MoveBy::create(0.5f, Vec2(0, -size.height)));
	});
	m_shop = MenuItemImage::create("levelselectscene/shop_normal.png",
		"levelselectscene/shop_selected.png",
		[this](Ref*)->void{
		//call shoplayer
		CCLOG("call shop");
	});
	auto mainMenu = Menu::create(m_seasons, m_heroHall, m_shop,nullptr);
	mainMenu->alignItemsHorizontally();
	mainMenu->setPosition(150, 75);
	this->addChild(mainMenu);

}

