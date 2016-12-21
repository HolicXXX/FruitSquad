#ifndef _LOADING_SCENE_H_
#define _LOADING_SCENE_H_

#include "cocos2d.h"

enum NextSceneType
{
	POINT_LELECT_SCENE,
	GAME_SCENE
};

class LoadingScene : public cocos2d::Layer
{
public:
	static LoadingScene* create();
	virtual bool init();
	virtual void onExit();
	virtual void onEnterTransitionDidFinish();

	virtual void update(float dt);
	void setNextSceneAni(NextSceneType type, int num);
	void bindNextSceneCallBack(const std::function<cocos2d::Scene*()>& func){ m_callback = func; };
	void aniLoadtCallBack(float);
private:
	std::function<cocos2d::Scene*()> m_callback;

	bool m_loadNext;
	NextSceneType m_nextSceneType;
	int m_nextSceneAniCountMax;
	int m_nextSceneAniCount;

	cocos2d::Label* m_text;

	cocos2d::Sprite* m_wcloud1;
	cocos2d::Sprite* m_wcloud2;
	cocos2d::Sprite* m_wcloud3;
	cocos2d::Sprite* m_wcloud4;
	cocos2d::Sprite* m_bcloud1;
	cocos2d::Sprite* m_bcloud2;
	cocos2d::Sprite* m_bcloud3;
	cocos2d::Sprite* m_bcloud4;
	void initBackGround();
	void bgUpdate(float dt);
};


#endif