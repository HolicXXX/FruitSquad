#include "LoadingScene.h"
#include "AnimationManager.h"

LoadingScene* LoadingScene::create()
{
	auto load = new LoadingScene();
	if (load && load->init())
	{
		load->autorelease();
		return load;
	}
	CC_SAFE_DELETE(load);
	return nullptr;
}

bool LoadingScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_callback = nullptr;
	auto size = Director::getInstance()->getVisibleSize();
	AnimationManager::getInstance()->loadLoadingSceneAni();

	initBackGround();
	//hero
	auto hero = Armature::create("loading");
	hero->getAnimation()->play("hero");
	hero->setPosition(size.width / 2, size.height / 3 * 2);
	this->addChild(hero);
	//loading text
	m_text = Label::createWithBMFont("fonts/loading_text.fnt", "Loading");
	m_text->setPosition(size / 2);
	this->addChild(m_text);
	//random delay time
	m_delay = CCRANDOM_0_1() * 3 + 1;

	this->scheduleUpdate();
	return true;
}

void LoadingScene::update(float dt)
{
	static float delay = 0;
	delay += dt;
	if (delay >= m_delay)
	{
		delay = 0;
		if (m_callback)
		{
			auto scene = m_callback();
			Director::getInstance()->replaceScene(TransitionFade::create(0.25f, scene));
			return;
		}
	}
	static float time = 0;
	time += dt;
	if (time >= 0.5f)
	{
		time = 0;
		auto str = m_text->getString();
		if (str.length() >= 10)
		{
			str = "Loading";
		}
		else
		{
			str.append(".");
		}
		m_text->setString(str);
	}
	bgUpdate(dt);
}

void LoadingScene::initBackGround()
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

void LoadingScene::bgUpdate(float dt)
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