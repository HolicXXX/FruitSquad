#ifndef _LOADING_SCENE_H_
#define _LOADING_SCENE_H_

#include "cocos2d.h"

class LoadingScene : public cocos2d::Layer
{
public:
	static LoadingScene* create();
	virtual bool init();

	virtual void update(float dt);
	void bindNextSceneCallBack(const std::function<cocos2d::Scene*()>& func){ m_callback = func; };
private:
	std::function<cocos2d::Scene*()> m_callback;
	float m_delay;

	cocos2d::Label* m_text;

	cocos2d::Sprite* m_wcloud1;
	cocos2d::Sprite* m_wcloud2;
	cocos2d::Sprite* m_wcloud3;
	cocos2d::Sprite* m_wcloud4;

	cocos2d::Sprite* m_bcloud1;
	cocos2d::Sprite* m_bcloud2;
	cocos2d::Sprite* m_bcloud3;
	cocos2d::Sprite* m_bcloud4;
};


#endif