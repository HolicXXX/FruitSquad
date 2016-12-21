#include "AnimationManager.h"
#include "LoadingScene.h"

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
	ArmatureDataManager::getInstance()->addArmatureFileInfo("startscene/start.ExportJson");
}

void AnimationManager::loadLevelSelectSceneAni(Layer* ref,int index)
{
	/*switch (index)
	{
	case 0:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("levelselectscene/shop/ani/shopleft.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 1:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("levelselectscene/shop/ani/shopright.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	default:
		break;
	}*/

	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("levelselectscene/shop/ani/shopleft.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("levelselectscene/shop/ani/shopright.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
}

void AnimationManager::loadLoadingSceneAni()
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo("loadingscene/loading.ExportJson");
}

void AnimationManager::loadGameSceneAni(int level, Layer* ref, int index)
{
	/*switch (index)
	{
	case 0:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/start/start_count_down.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 1:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/orange/orange.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 2:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/orange/orangeskill.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 3:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/orange/orangeBulletEff.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 4:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/orange/orangeSkillEff.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 5:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/coconut/coconut.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 6:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/coconut/coconutSkillEff.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 7:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/coconut/coconutSkillBuff.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 8:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/coconut/coconutBulletEff.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 9:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/apple/apple.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 10:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/apple/appleSkill.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 11:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/enemy/scarab/scarab.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 12:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/enemy/scarab/scarabBullet.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 13:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/burn/burn.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 14:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/gold/gold.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 15:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/get/get.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 16:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/upgrade/upgrade.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 17:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/heal/heal.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 18:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/ice/ice.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	case 19:
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/freeze/freeze.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	}
	break;
	default:
		break;
	}*/

	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/start/start_count_down.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));

	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/orange/orange.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/orange/orangeskill.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/orange/orangeBulletEff.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/orange/orangeSkillEff.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));

	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/coconut/coconut.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/coconut/coconutSkillEff.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/coconut/coconutSkillBuff.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/coconut/coconutBulletEff.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));

	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/apple/apple.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/hero/apple/appleSkill.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));

	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/enemy/scarab/scarab.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/enemy/scarab/scarabBullet.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));

	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/burn/burn.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/gold/gold.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/get/get.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/upgrade/upgrade.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/heal/heal.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/ice/ice.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("gamescene/ani/eff/freeze/freeze.ExportJson", ref, SEL_SCHEDULE(&LoadingScene::aniLoadtCallBack));
}

void AnimationManager::eraseStartSceneAni()
{
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("startscene/start.ExportJson");
}

void AnimationManager::eraseLevelSelectSceneAni()
{
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("levelselectscene/shop/ani/shopleft.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("levelselectscene/shop/ani/shopright.ExportJson");
}

void AnimationManager::eraseLoadingSceneAni()
{
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("loadingscene/loading.ExportJson");
}

void AnimationManager::eraseGameSceneAni(int level)
{
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/start/start_count_down.ExportJson");

	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/hero/orange/orange.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/hero/orange/orangeskill.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/hero/orange/orangeBulletEff.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/hero/orange/orangeSkillEff.ExportJson");

	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/hero/coconut/coconut.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/hero/coconut/coconutSkillEff.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/hero/coconut/coconutSkillBuff.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/hero/coconut/coconutBulletEff.ExportJson");

	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/hero/apple/apple.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/hero/apple/appleSkill.ExportJson");

	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/enemy/scarab/scarab.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/enemy/scarab/scarabBullet.ExportJson");

	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/eff/burn/burn.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/eff/gold/gold.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/eff/get/get.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/eff/upgrade/upgrade.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/eff/heal/heal.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/eff/ice/ice.ExportJson");
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("gamescene/ani/eff/freeze/freeze.ExportJson");
}