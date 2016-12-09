#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_

#include "HeadFile.h"

class AnimationManager
{
public:
	static AnimationManager* getInstance();

	void loadStartSceneAni();
	void loadLoadingSceneAni();
	void loadGameSceneAni(int level);
};


#endif