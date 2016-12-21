#include "ShopLayer.h"
#include "JsonTool.h"
#include "TipsLayer.h"

ShopLayer* ShopLayer::create()
{
	auto sl = new ShopLayer();
	if (sl && sl->init())
	{
		sl->autorelease();
		return sl;
	}
	CC_SAFE_DELETE(sl);
	return nullptr;
}

bool ShopLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto size = Director::getInstance()->getVisibleSize();
	m_pages = { "ability", "magic", "hp", "gem" };
	m_title = nullptr;
	m_titleLabel = nullptr;
	this->setPosition(Vec2(0, -size.height));

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
				auto seq = Sequence::create(
					MoveBy::create(0.3f, Vec2(0, -size.height)),
					nullptr);
				this->runAction(seq);
			}
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(lis, m_returnButton);
		this->addChild(m_returnButton);
	}
	initShopBG();
	initListView();
	initPageView();
	resetTitle(m_focus);
	initGems();

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

	initAni();
	this->scheduleUpdate();
	return true;
}

void ShopLayer::onExit()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
	Node::onExit();
}

void ShopLayer::resetTitle(int p)
{
	if (!m_title)
	{
		m_title = Sprite::create("levelselectscene/shop/title.png");
		m_title->setPosition(446, 580);
		m_bg->addChild(m_title);
		m_titleLabel = Sprite::create(StringUtils::format("levelselectscene/shop/title_label_%s.png", m_pages[p].c_str()));
		m_titleLabel->setPosition(m_title->getContentSize() / 2);
		m_title->addChild(m_titleLabel);
		return;
	}
	m_titleLabel->setTexture(StringUtils::format("levelselectscene/shop/title_label_%s.png", m_pages[p].c_str()));
}

void ShopLayer::initGems()
{
	m_gems = Node::create();
	{
		auto bg = Sprite::create("levelselectscene/herohall/gems_number_bg.png");
		bg->setTag(1);
		m_gems->addChild(bg);
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
		//add observer
		NotificationCenter::getInstance()->addObserver(this, SEL_CallFuncO(&ShopLayer::costGems), "costGems", nullptr);
	}
	m_gems->setPosition(740, 550);
	m_bg->addChild(m_gems);
}

void ShopLayer::costGems(Ref* b)
{
	auto gemNum = JsonTool::getInstance()->getDoc()["gems"].GetInt();
	auto button = static_cast<Button*>(b);
	auto index = button->getTag();
	int page = m_page->getCurrentPageIndex();
	int cost = JsonTool::getInstance()->getDoc()["shop"][page]["button"][index]["gems"].GetInt();
	if (cost <= gemNum)
	{
		gemNum -= cost;
		JsonTool::getInstance()->getDoc()["gems"].SetInt(gemNum);
		buttonCallBack(page, index);
		static_cast<Label*>(m_gems->getChildByTag(2))->setString(StringUtils::format("%d", gemNum));
	}
	else
	{
		this->addChild(TipsLayer::getInstance()->getNEGemsLayer());
	}
}

void ShopLayer::buttonCallBack(int page, int index)
{
	switch (page)
	{
	case 0:
	{
		switch (index)
		{
		case 0:
		{
			auto ori = JsonTool::getInstance()->getDoc()["fruit"].GetInt();
			JsonTool::getInstance()->getDoc()["fruit"].SetInt(ori + 5);
		}
		break;
		case 1:
		{
			auto ori = JsonTool::getInstance()->getDoc()["hpbottlemax"].GetInt();
			JsonTool::getInstance()->getDoc()["hpbottlemax"].SetInt(ori + 1);
		}
		break;
		case 2:
		{//TODO
		}
		break;
		default:
			break;
		}
	}
		break;
	case 1:
	{
		auto ori = JsonTool::getInstance()->getDoc()["items"][index]["num"].GetInt();
		JsonTool::getInstance()->getDoc()["items"][index]["num"].SetInt(ori + 1);
	}
		break;
	case 2:
		break;
	case 3:
	{
		//int add = JsonTool::getInstance()->getDoc()["shop"][page]["button"][index]["gems"].GetInt();
		//auto ori = JsonTool::getInstance()->getDoc()["gems"].GetInt();
		//JsonTool::getInstance()->getDoc()["gems"].SetInt(ori + add);
	}
		break;
	default:
		break;
	}
}

void ShopLayer::initPageView()
{
	m_page = PageView::create();
	m_page->setSize(Size(630, 290));
	m_page->setPosition(Vec2(120, 171));

	m_page->addPage(getPage(0));//
	m_page->addPage(getPage(1));//
	m_page->addPage(getPage(2));//
	m_page->addPage(getPage(3));//
	m_page->setCurrentPageIndex(m_focus);
	m_bg->addChild(m_page);
}

ListView* ShopLayer::getPage(int index)
{
	auto itemnum = JsonTool::getInstance()->getDoc()["shop"][index]["button"].Size();
	auto page = ListView::create();
	page->setDirection(ScrollView::Direction::HORIZONTAL);
	for (int i = 0; i < itemnum; ++i)
	{
		String str = JsonTool::getInstance()->getDoc()["shop"][index]["button"][i]["name"].GetString();
		auto bg = ImageView::create("levelselectscene/shop/item_bg.png");
		auto icon = ImageView::create(StringUtils::format("levelselectscene/shop/icon_%s.png", str.getCString()));
		icon->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 3 * 2));
		bg->addChild(icon);
		if (index < 3)
		{
			auto button = Button::create("levelselectscene/shop/buy_gem_normal.png", "levelselectscene/shop/buy_gem_selected.png", "levelselectscene/shop/buy_disable.png");
			button->setTag(i);
			button->addTouchEventListener([](Ref* b, Widget::TouchEventType type)->void{
				if (type == Widget::TouchEventType::ENDED)
					NotificationCenter::getInstance()->postNotification("costGems", b);
			});
			button->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 5));
			bg->addChild(button);
			auto num = JsonTool::getInstance()->getDoc()["shop"][index]["button"][i]["gems"].GetInt();
			auto text = Text::create(StringUtils::format("%d", num), "fonts/arial.ttf", 24);
			text->setPosition(button->getContentSize() / 2);
			button->addChild(text);
		}
		else
		{
			auto button = Button::create("levelselectscene/shop/buy_rmb_normal.png", "levelselectscene/shop/buy_rmb_selected.png", "levelselectscene/shop/buy_disable.png");
			button->setTag(i);
			button->addTouchEventListener([](Ref* b, Widget::TouchEventType type)->void{
				if (type == Widget::TouchEventType::ENDED)
					NotificationCenter::getInstance()->postNotification("costGems", b);
			});
			button->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 5));
			bg->addChild(button);
			auto num = JsonTool::getInstance()->getDoc()["shop"][index]["button"][i]["rmb"].GetInt();
			auto text = Text::create(StringUtils::format("%d", num), "fonts/arial.ttf", 24);
			text->setPosition(button->getContentSize() / 2);
			button->addChild(text);
		}
		page->setItemModel(bg);
		page->pushBackDefaultItem();
	}
	page->setSize(Size(630, 290));
	page->setItemsMargin(0);
	page->setScrollBarEnabled(false);
	return page;
}

void ShopLayer::initListView()
{
	m_listView = ListView::create();
	m_listView->setDirection(ScrollView::Direction::HORIZONTAL);
	for (int i = 0; i < 4; ++i)
	{
		auto checkBox = CheckBox::create();
		checkBox->setTag(i);
		checkBox->loadTextureBackGround(StringUtils::format("levelselectscene/shop/button_%s_normal.png", m_pages[i].c_str()));
		checkBox->loadTextureFrontCross(StringUtils::format("levelselectscene/shop/button_%s_selected.png", m_pages[i].c_str()));
		checkBox->loadTextureBackGroundSelected(StringUtils::format("levelselectscene/shop/button_%s_selected.png", m_pages[i].c_str()));
		checkBox->addEventListenerCheckBox(this, checkboxselectedeventselector(ShopLayer::checkBoxCallBack));
		if (i == 0)
		{
			checkBox->setSelected(true);
			m_focus = 0;
		}
		m_listView->setItemModel(checkBox);
		m_listView->pushBackDefaultItem();
	}
	m_listView->setSize(Size(600, 150));
	m_listView->setItemsMargin(20);
	m_listView->setScrollBarEnabled(false);
	m_listView->setPosition(Vec2(175, 25));
	m_bg->addChild(m_listView);
}

void ShopLayer::checkBoxCallBack(Ref* sender, CheckBoxEventType type)
{
	auto checkbox = static_cast<CheckBox*>(sender);
	switch (type)
	{
	case CheckBoxEventType::CHECKBOX_STATE_EVENT_SELECTED:
	{
		auto childs = m_listView->getItems();
		for (int i = 0; i < childs.size(); ++i)
		{
			auto c = static_cast<CheckBox*>(childs.at(i));
			if (c->getTag() == checkbox->getTag())
			{
				continue;
			}
			c->setSelectedState(false);
		}
		m_focus = checkbox->getTag();
		resetTitle(m_focus);
		m_page->setCurPageIndex(m_focus);
	}
	break;
	case CheckBoxEventType::CHECKBOX_STATE_EVENT_UNSELECTED:
	{
		if (checkbox->getTag() == m_focus)
			checkbox->setSelectedState(true);
	}
	break;
	default:
		break;
	}
}

void ShopLayer::initShopBG()
{
	auto size = Director::getInstance()->getVisibleSize();
	m_bg = Sprite::create("levelselectscene/shop/shop_bg.png");
	m_bg->setPosition(size / 2);
	this->addChild(m_bg);
}

void ShopLayer::initAni()
{
	m_leftAni = Armature::create("shopleft");
	m_leftAni->getAnimation()->play("guava");
	m_leftAni->setScaleX(-1);
	m_leftAni->setPosition(120, 68);
	m_bg->addChild(m_leftAni);

	m_rightAni = Armature::create("shopright");
	m_rightAni->getAnimation()->play("coconut");
	m_rightAni->setPosition(795, 68);
	m_bg->addChild(m_rightAni);
}

void ShopLayer::update(float dt)
{
	bgUpdate(dt);
}

void ShopLayer::initBackGround()
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

void ShopLayer::bgUpdate(float dt)
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