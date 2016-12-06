#include "UIButton.h"
USING_NS_CC;

UIButton* UIButton::create(ButtonType type)
{
	auto b = new UIButton();
	if (b && b->init(type))
	{
		b->autorelease();
		return b;
	}
	CC_SAFE_DELETE(b);
	return nullptr;
}

bool UIButton::init(ButtonType type)
{
	if (!Node::init())
	{
		return false;
	}
	m_type = type;
	initButton();
	initListener();

	return true;
}

void UIButton::resetState()
{
	switch (m_type)
	{
	case PAUSE:
	{
		setType(CONTINUE);
	}
		break;
	case CONTINUE:
	{
		setType(PAUSE);
	}
		break;
	case SPEED_1:
	{
		setType(SPEED_2);
	}
		break;
	case SPEED_2:
	{
		setType(SPEED_1);
	}
		break;
	case MENU_SOUND_ON:
	{
		setType(MENU_SOUND_OFF);
	}
		break;
	case MENU_SOUND_OFF:
	{
		setType(MENU_SOUND_ON);
	}
		break;
	case MENU_MUSIC_ON:
	{
		setType(MENU_MUSIC_OFF);
	}
		break;
	case MENU_MUSIC_OFF:
	{
		setType(MENU_MUSIC_ON);
	}
		break;
	default:
		break;
	}
}

void UIButton::initListener()
{
	m_listener = EventListenerTouchOneByOne::create();
	m_listener->setSwallowTouches(true);
	m_listener->onTouchBegan = [this](Touch* t, Event* e)->bool{
		auto pos = this->convertTouchToNodeSpace(t);
		auto bsize = m_button->getContentSize();
		auto rc = Rect{ -Vec2(bsize / 2), bsize };
		if (rc.containsPoint(pos))
		{
			setSelected();
			return true;
		}
		return false;
	};
	m_listener->onTouchEnded = [this](Touch* t, Event* e)->void{
		auto pos = this->convertTouchToNodeSpace(t);
		auto bsize = m_button->getContentSize();
		auto rc = Rect{ -Vec2(bsize / 2), bsize };
		if (rc.containsPoint(pos))
		{
			resetState();
			setNormal();
			if (m_type == PAUSE || m_type == SPEED_1 || m_type == MENU_SOUND_ON || m_type == MENU_MUSIC_ON)
			{
				m_callback2();
			}
			else
				m_callback();
		}
		else
			setNormal();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
}

void UIButton::initButton()
{
	std::string filename = "gamescene/ui/";
	switch (m_type)
	{
	case PAUSE:
	{
		m_name = "pause";
	}
		break;
	case SPEED_1:
	{
		m_name = "speed_01";
	}
		break;
	case SPEED_2:
	{
		m_name = "speed_02";
	}
		break;
	case MENU:
	{
		m_name = "menu";
	}
		break;
	case MENU_CONTINUE:
	{
		m_name = "menu_continue";
	}
		break;
	case MENU_RETRY:
	{
		m_name = "menu_retry";
	}
		break;
	case MENU_SOUND_ON:
	{
		m_name = "menu_soundon";
	}
		break;
	case MENU_SOUND_OFF:
	{
		m_name = "menu_soundoff";
	}
		break;
	case MENU_MUSIC_ON:
	{
		m_name = "menu_musicon";
	}
		break;
	case MENU_MUSIC_OFF:
	{
		m_name = "menu_musicoff";
	}
		break;
	case MENU_RETURN:
	{
		m_name = "menu_return";
	}
		break;
	default:
		break;
	}
	m_button = Sprite::create(StringUtils::format("%s%s_normal.png", filename.c_str(), m_name.c_str()));
	m_button->setTag(1);
	this->addChild(m_button);
}

void UIButton::setType(ButtonType t)
{
	switch (t)
	{
	case PAUSE:
	{
		m_name = "pause";
	}
	break;
	case CONTINUE:
	{
		m_name = "continue";
	}
	break;
	case SPEED_1:
	{
		m_name = "speed_01";
	}
	break;
	case SPEED_2:
	{
		m_name = "speed_02";
	}
	break;
	case MENU:
	{
		m_name = "menu";
	}
	break;
	case MENU_CONTINUE:
	{
		m_name = "menu_continue";
	}
	break;
	case MENU_RETRY:
	{
		m_name = "menu_retry";
	}
	break;
	case MENU_SOUND_ON:
	{
		m_name = "menu_soundon";
	}
	break;
	case MENU_SOUND_OFF:
	{
		m_name = "menu_soundoff";
	}
	break;
	case MENU_MUSIC_ON:
	{
		m_name = "menu_musicon";
	}
	break;
	case MENU_MUSIC_OFF:
	{
		m_name = "menu_musicoff";
	}
	break;
	case MENU_RETURN:
	{
		m_name = "menu_return";
	}
	break;
	default:
		break;
	}
	m_type = t;
}

void UIButton::setNormal()
{
	m_button->setTexture(StringUtils::format("gamescene/ui/%s_normal.png", m_name.c_str()));
}

void UIButton::setSelected()
{
	m_button->setTexture(StringUtils::format("gamescene/ui/%s_selected.png", m_name.c_str()));
}