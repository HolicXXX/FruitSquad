#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_

class AnimationManager
{
public:
	static AnimationManager* getInstance();

	void loadStartSceneAni();
	void loadLoadingSceneAni();
	void loadGameSceneAni();
};


#endif