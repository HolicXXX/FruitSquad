#include "AnimationManager.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace cocostudio;

static AnimationManager* _aniManager = nullptr;

AnimationManager* AnimationManager::getInstance()
{
	if (!_aniManager)
	{
		_aniManager = new AnimationManager();
	}
	return _aniManager;
}

void AnimationManager::loadStartSceneAni()
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo("startscene/start0.png", "startscene/start0.plist", "startscene/start.ExportJson");
}

void AnimationManager::loadLoadingSceneAni()
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo("loadingscene/loading0.png", "loadingscene/loading0.plist", "loadingscene/loading.ExportJson");
}

void AnimationManager::loadGameSceneAni()
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/start/start_count_down.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/hero/orange/orange.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/hero/orange/orangeskill.ExportJson");
}