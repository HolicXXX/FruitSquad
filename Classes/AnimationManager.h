#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_

#include "HeadFile.h"

class AnimationManager
{
public:
	static AnimationManager* getInstance();

	void loadStartSceneAni();
	void loadLevelSelectSceneAni(Layer*,int);
	void loadLoadingSceneAni();
	void loadGameSceneAni(int level,Layer*,int);

	void eraseStartSceneAni();
	void eraseLevelSelectSceneAni();
	void eraseLoadingSceneAni();
	void eraseGameSceneAni(int level);
private:
	void post(float);
};


#endif