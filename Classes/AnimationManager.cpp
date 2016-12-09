#include "AnimationManager.h"

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

void AnimationManager::loadGameSceneAni(int level)
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/start/start_count_down.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/hero/orange/orange.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/hero/orange/orangeskill.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/hero/orange/orangeBulletEff.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/hero/orange/orangeSkillEff.ExportJson");

	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/enemy/scarab/scarab.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/enemy/scarab/scarabBullet.ExportJson");

	ArmatureDataManager::getInstance()->addArmatureFileInfo("gamescene/ani/eff/burn/burn.ExportJson");
}