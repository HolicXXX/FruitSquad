#ifndef _BOXCULLISION_MANAGER_H_
#define _BOXCULLISION_MANAGER_H_

#include "BoxBase.h"

class BoxCullisionManager
{
public:
	static BoxCullisionManager* getInstance();

	void registHeroBox(Vector<BoxBase*> vhb);
	void registEnemyBox(Vector<BoxBase*> veb);
	void addHeroBox(BoxBase* h);
	void addEnemyBox(BoxBase* e);
	void eraseHeroBox(BoxBase* h);
	void eraseEnemyBox(BoxBase* e);
	void removeAllHeorBox();
	void removeAllEnemyBox();
	bool hitEnemy(BoxBase* h);
	bool hitHero(BoxBase* e);

	Vector<BoxBase*> m_heroBox;
	Vector<BoxBase*> m_EnemyBox;
};


#endif