#ifndef _GAMEMENU_LAYER_H_
#define _GAMEMENU_LAYER_H_

#include "cocos2d.h"
#include "UIButton.h"

class GameMenuLayer : public cocos2d::Layer
{
public:
	static GameMenuLayer* create();
	virtual bool init();

	void bindContinueCallBack(const std::function<void()>& func){ m_continueCallback = func; }
	void bindRetryCallBack(const std::function<void()>& func){ m_retryCallback = func; }
	void bindReturnCallBack(const std::function<void()>& func){ m_returnCallback = func; }
private:
	cocos2d::Sprite* m_bg;
	void initContinue();
	UIButton* m_continue;
	std::function<void()> m_continueCallback;
	
	void initRetry();
	UIButton* m_retry;
	std::function<void()> m_retryCallback;
	
	void initSound();
	UIButton* m_sound;

	void initMusic();
	UIButton* m_music;

	void initReturn();
	UIButton* m_return;
	std::function<void()> m_returnCallback;
};


#endif