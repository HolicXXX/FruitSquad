#include "SeasonLayer.h"
#include "JsonTool.h"
USING_NS_CC;

SeasonLayer* SeasonLayer::create()
{
	auto layer = new SeasonLayer();
	if (layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool SeasonLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto size = Director::getInstance()->getVisibleSize();
	m_focusIndex = 0;
	m_callback = nullptr;

	//render
	auto rt = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
	rt->setPosition(size / 2);
	rt->clear(0, 0, 0, 0.6f);
	this->addChild(rt);

	//init backlight
	m_backLight = Sprite::create("levelselectscene/season/bg_light.png");
	m_backLight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	m_backLight->setPosition(size.width / 2, size.height);
	this->addChild(m_backLight);
	//init title
	m_title = Node::create();
	//base on data
	{
		auto back = Sprite::create("levelselectscene/season/title_bg_gold.png");
		back->setTag(1);
		m_title->addChild(back);
		auto label = Sprite::create("levelselectscene/season/title_label_selected.png");
		label->setTag(2);
		m_title->addChild(label);
	}
	m_title->setPosition(size.width / 2, size.height - 100);
	this->addChild(m_title);
	//init seasons
	initSeasons();
	//add lis
	auto lis = EventListenerTouchOneByOne::create();
	lis->setSwallowTouches(true);
	lis->onTouchBegan = [this](Touch* t, Event* e)->bool{
		auto focus = m_seasons.at(m_focusIndex);
		auto pic = focus->getChildByTag(1);
		auto pos = pic->convertTouchToNodeSpace(t);
		Rect rc = { Vec2::ZERO, pic->getContentSize() };
		if (rc.containsPoint(pos))
		{
			focus->getChildByTag(2)->setVisible(true);
		}
		return true;
	};
	lis->onTouchMoved = [this](Touch* t, Event* e)->void{
		auto focus = m_seasons.at(m_focusIndex);
		auto pic = focus->getChildByTag(1);
		auto pos = pic->convertTouchToNodeSpace(t);
		Rect rc = { Vec2::ZERO, pic->getContentSize() };
		if (!rc.containsPoint(pos))
		{
			focus->getChildByTag(2)->setVisible(false);
		}
		else
		{
			focus->getChildByTag(2)->setVisible(true);
		}
		this->move(t->getDelta().x);
	};
	lis->onTouchEnded = [this](Touch* t, Event* e)->void{
		auto focus = m_seasons.at(m_focusIndex);
		focus->getChildByTag(2)->setVisible(false);
		if ((t->getStartLocation() - t->getLocation()).length() <= 10)
		{
			auto pic = focus->getChildByTag(1);
			auto pos = pic->convertTouchToNodeSpace(t);
			Rect rc = { Vec2::ZERO, pic->getContentSize() };
			if (rc.containsPoint(pos))
			{
				if (focus->getChildByTag(3) == nullptr)
					this->removeFromParent();
			}
			else
				this->removeFromParent();
		}
		else
		{
			changeFocus();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, this);

	return true;
}

void SeasonLayer::changeFocus()
{
	auto size = Director::getInstance()->getVisibleSize();
	auto focus = m_seasons.at(m_focusIndex);
	auto posX = focus->getPositionX();
	auto offset = posX - size.width / 2;
	if ((offset < -10) && (m_focusIndex != (m_seasons.size() - 1)))
	{
		m_focusIndex = m_focusIndex + 1;
	}
	else if ((offset > 10) && (m_focusIndex != 0))
	{
		m_focusIndex = m_focusIndex - 1;
	}
	auto newFocus = m_seasons.at(m_focusIndex);
	auto newOffset = size.width / 2 - newFocus->getPositionX();
	for (int i = 0; i < m_seasons.size(); ++i)
	{
		auto s = m_seasons.at(i);
		auto duration = abs(newOffset / size.width / 2 * 0.5f);
		s->runAction(Sequence::create(MoveBy::create(duration, Vec2(newOffset, 0)),
			CallFunc::create([s, newFocus, i]()->void{
			if (s == newFocus)
			{
				switch (i)
				{
				case 0:
				{
					auto pic = static_cast<Sprite*>(s->getChildByTag(1));
					pic->setTexture("levelselectscene/season/spring_normal.png");
				}
				break;
				case 1:
				{
					auto pic = static_cast<Sprite*>(s->getChildByTag(1));
					pic->setTexture("levelselectscene/season/summer_normal.png");
				}
				break;
				case 2:
				{
					auto pic = static_cast<Sprite*>(s->getChildByTag(1));
					pic->setTexture("levelselectscene/season/tobe_normal.png");
				}
				break;
				}
			}
			else
			{
				switch (i)
				{
				case 0:
				{
					auto pic = static_cast<Sprite*>(s->getChildByTag(1));
					pic->setTexture("levelselectscene/season/spring_shadow.png");
				}
				break;
				case 1:
				{
					auto pic = static_cast<Sprite*>(s->getChildByTag(1));
					pic->setTexture("levelselectscene/season/summer_shadow.png");
				}
				break;
				case 2:
				{
					auto pic = static_cast<Sprite*>(s->getChildByTag(1));
					pic->setTexture("levelselectscene/season/tobe_shadow.png");
				}
				break;
				}
			}
		}), nullptr));
	}
}

void SeasonLayer::move(float deltaX)
{
	for (auto s : m_seasons)
	{
		s->setPositionX(s->getPositionX() + deltaX);
	}
}

void SeasonLayer::initSeasons()
{
	if (m_seasons.size() > 0)
	{
		for (auto s : m_seasons)
			s->removeFromParent();
		m_seasons.clear();
	}
	auto size = Director::getInstance()->getVisibleSize();

	String springstate = JsonTool::getInstance()->getDoc()["spring"].GetString();
	auto spring = Node::create();
	{
		auto spr = Sprite::create("levelselectscene/season/spring_normal.png");
		spr->setTag(1);
		spring->addChild(spr);
		auto eff = Sprite::create("levelselectscene/season/shiny.png");
		eff->setTag(2);
		eff->setVisible(false);
		spring->addChild(eff);
		if (springstate.compare("lock") == 0)
		{
			auto lock = Sprite::create("levelselectscene/season/season_lock_01.png");
			lock->setTag(3);
			auto locklis = EventListenerTouchOneByOne::create();
			locklis->setSwallowTouches(true);
			locklis->onTouchBegan = [lock](Touch* t, Event* e)->bool{
				auto pos = lock->convertTouchToNodeSpace(t);
				auto rc = Rect{ Vec2::ZERO, lock->getContentSize() };
				if (rc.containsPoint(pos))
				{
					lock->setTexture("levelselectscene/season/season_lock_02.png");
					return true;
				}
				return false;
			};
			locklis->onTouchEnded = [lock](Touch* t, Event* E)->void{
				auto pos = lock->convertTouchToNodeSpace(t);
				auto rc = Rect{ Vec2::ZERO, lock->getContentSize() };
				if (rc.containsPoint(pos))
				{
					lock->setTexture("levelselectscene/season/season_lock_03.png");
					lock->runAction(Sequence::create(
						MoveBy::create(0.1f,Vec2(10,10)), 
						MoveBy::create(0.1f, Vec2(-20, -20)),
						MoveBy::create(0.1f, Vec2(10, 10)),
						CallFunc::create([lock]()->void{
						lock->setTexture("levelselectscene/season/season_lock_01.png");
					}),
						nullptr));
				}
				lock->setTexture("levelselectscene/season/season_lock_01.png");
			};
			_eventDispatcher->addEventListenerWithSceneGraphPriority(locklis, lock);
			spring->addChild(lock);
		}
	}
	spring->setPosition(size / 2);
	m_seasons.pushBack(spring);
	this->addChild(spring);

	String summerstate = JsonTool::getInstance()->getDoc()["summer"].GetString();
	auto summer = Node::create();
	{
		auto sum = Sprite::create("levelselectscene/season/summer_shadow.png");
		sum->setTag(1);
		summer->addChild(sum);
		auto eff = Sprite::create("levelselectscene/season/shiny.png");
		eff->setTag(2);
		eff->setVisible(false);
		summer->addChild(eff);
		if (summerstate.compare("lock") == 0)
		{
			auto lock = Sprite::create("levelselectscene/season/season_lock_01.png");
			lock->setTag(3);
			auto locklis = EventListenerTouchOneByOne::create();
			locklis->setSwallowTouches(true);
			locklis->onTouchBegan = [lock](Touch* t, Event* e)->bool{
				auto pos = lock->convertTouchToNodeSpace(t);
				auto rc = Rect{ Vec2::ZERO, lock->getContentSize() };
				if (rc.containsPoint(pos))
				{
					lock->setTexture("levelselectscene/season/season_lock_02.png");
					return true;
				}
				return false;
			};
			locklis->onTouchEnded = [lock](Touch* t, Event* E)->void{
				auto pos = lock->convertTouchToNodeSpace(t);
				auto rc = Rect{ Vec2::ZERO, lock->getContentSize() };
				if (rc.containsPoint(pos))
				{
					lock->setTexture("levelselectscene/season/season_lock_03.png");
					lock->runAction(Sequence::create(
						MoveBy::create(0.1f, Vec2(10, 10)),
						MoveBy::create(0.1f, Vec2(-20, -20)),
						MoveBy::create(0.1f, Vec2(10, 10)),
						CallFunc::create([lock]()->void{
						lock->setTexture("levelselectscene/season/season_lock_01.png");
					}),
						nullptr));
				}
				lock->setTexture("levelselectscene/season/season_lock_01.png");
			};
			_eventDispatcher->addEventListenerWithSceneGraphPriority(locklis, lock);
			summer->addChild(lock);
		}
	}
	summer->setPosition(spring->getPosition() + Vec2(size.width / 2, 0));
	m_seasons.pushBack(summer);
	this->addChild(summer);

	auto tobe = Node::create();
	{
		auto t = Sprite::create("levelselectscene/season/tobe_shadow.png");
		t->setTag(1);
		tobe->addChild(t);
		auto eff = Sprite::create("levelselectscene/season/shiny.png");
		eff->setTag(2);
		eff->setVisible(false);
		tobe->addChild(eff);
		auto lock = Sprite::create("levelselectscene/season/season_lock_01.png");
		lock->setTag(3);
		auto locklis = EventListenerTouchOneByOne::create();
		locklis->setSwallowTouches(true);
		locklis->onTouchBegan = [lock](Touch* t, Event* e)->bool{
			auto pos = lock->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, lock->getContentSize() };
			if (rc.containsPoint(pos))
			{
				lock->setTexture("levelselectscene/season/season_lock_02.png");
				return true;
			}
			return false;
		};
		locklis->onTouchEnded = [lock](Touch* t, Event* E)->void{
			auto pos = lock->convertTouchToNodeSpace(t);
			auto rc = Rect{ Vec2::ZERO, lock->getContentSize() };
			if (rc.containsPoint(pos))
			{
				lock->setTexture("levelselectscene/season/season_lock_03.png");
				lock->runAction(Sequence::create(
					MoveBy::create(0.1f, Vec2(10, 10)),
					MoveBy::create(0.1f, Vec2(-20, -20)),
					MoveBy::create(0.1f, Vec2(10, 10)),
					CallFunc::create([lock]()->void{
					lock->setTexture("levelselectscene/season/season_lock_01.png");
				}),
					nullptr));
			}
			lock->setTexture("levelselectscene/season/season_lock_01.png");
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(locklis, lock);
		tobe->addChild(lock);
	}
	tobe->setPosition(summer->getPosition() + Vec2(size.width / 2, 0));
	m_seasons.pushBack(tobe);
	this->addChild(tobe);
}

