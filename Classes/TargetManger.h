#ifndef _TARGETMANAMER_H_
#define _TARGETMANAMER_H_

#include "HeroBase.h"
#include "EnemyBase.h"


class TargetManager
{
public:
	static TargetManager* getInstance();

	void registHero(Vector<HeroBase*> vh);
	void registEnemy(Vector<EnemyBase*> ve);
	void removeHeor();
	void removeEnemy();

	TargetBase* getEnemyTarget(TargetBase* h);
	TargetBase* getHeroTarget(TargetBase* e);

private:

	Vector<HeroBase*> m_heros;
	Vector<EnemyBase*> m_enemy;

};


#endif