#include "GameMenuLayer.h"
USING_NS_CC;

GameMenuLayer* GameMenuLayer::create()
{
	auto l = new GameMenuLayer();
	if (l && l->init())
	{
		l->autorelease();
		return l;
	}
	CC_SAFE_DELETE(l);
	return nullptr;
}

bool GameMenuLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto size = Director::getInstance()->getVisibleSize();

	//rt
	auto rt = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
	rt->setPosition(size / 2);
	rt->setName("rt");
	rt->clear(0, 0, 0, 0.5f);
	this->addChild(rt);
	auto swa = EventListenerTouchOneByOne::create();
	swa->setSwallowTouches(true);
	swa->onTouchBegan = [this](Touch* t, Event* e)->bool{
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swa, rt);

	//bg
	m_bg = Sprite::create("gamescene/ui/menu_bg.png");
	m_bg->setTag(1);
	m_bg->setPosition(size / 2);
	this->addChild(m_bg);

	initContinue();
	initRetry();
	initSound();
	initMusic();
	initReturn();

	return true;
}

void GameMenuLayer::initContinue()
{
	m_continue = UIButton::create(ButtonType::MENU_CONTINUE);
	m_continue->bindCallBack([this]()->void{
		m_continueCallback();
		this->removeFromParent();
	});
	m_continue->setPosition(201, 73);
	m_bg->addChild(m_continue);
}

void GameMenuLayer::initRetry()
{
	m_retry = UIButton::create(ButtonType::MENU_RETRY);
	m_retry->bindCallBack([this]()->void{
		m_retryCallback();
		this->removeFromParent();
	});
	m_retry->setPosition(313, 73);
	m_bg->addChild(m_retry);
}

void GameMenuLayer::initSound()
{
	//data
	m_sound = UIButton::create(ButtonType::MENU_SOUND_ON);
	m_sound->bindCallBack([]()->void{
		//sound on - off
		CCLOG("sound on callback");
	});
	m_sound->bindSecCallBack([]()->void{
		//sound off - on
		CCLOG("sound off callback");
	});
	m_sound->setPosition(425, 73);
	m_bg->addChild(m_sound);
}

void GameMenuLayer::initMusic()
{
	m_music = UIButton::create(ButtonType::MENU_MUSIC_ON);
	m_music->bindCallBack([]()->void{
		//music on - off
		CCLOG("music on callback");
	});
	m_music->bindSecCallBack([]()->void{
		//music off - on
		CCLOG("music off callback");
	});
	m_music->setPosition(538, 73);
	m_bg->addChild(m_music);
}

void GameMenuLayer::initReturn()
{
	m_return = UIButton::create(ButtonType::MENU_RETURN);
	m_return->bindCallBack([this]()->void{
		m_returnCallback();
		this->removeFromParent();
	});
	m_return->setPosition(650, 73);
	m_bg->addChild(m_return);
}