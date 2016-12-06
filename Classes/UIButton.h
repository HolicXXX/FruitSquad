#ifndef _UIBUTTON_H_
#define _UIBUTTON_H_

#include "cocos2d.h"

enum ButtonType
{
	PAUSE,
	CONTINUE,
	SPEED_1,
	SPEED_2,
	MENU,
	MENU_CONTINUE,
	MENU_RETRY,
	MENU_SOUND_ON,
	MENU_SOUND_OFF,
	MENU_MUSIC_ON,
	MENU_MUSIC_OFF,
	MENU_RETURN
};

class UIButton : public cocos2d::Node
{
public:
	static UIButton* create(ButtonType type);
	virtual bool init(ButtonType type);

	ButtonType getType(){ return m_type; }
	void setType(ButtonType t);

	void setNormal();
	void setSelected();

	void bindCallBack(const std::function<void()> & func){ m_callback = func; }
	void bindSecCallBack(const std::function<void()> & func){ m_callback2 = func; }
private:
	void initButton();
	void initListener();
	cocos2d::EventListenerTouchOneByOne* m_listener;
	std::string m_name;
	ButtonType m_type;
	cocos2d::Sprite* m_button;
	std::function<void()> m_callback;
	std::function<void()> m_callback2;

	void resetState();
};



#endif