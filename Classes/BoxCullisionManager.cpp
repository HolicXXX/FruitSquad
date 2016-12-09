#include "BoxCullisionManager.h"
#include "TargetManger.h"
static BoxCullisionManager* _manager = nullptr;

BoxCullisionManager* BoxCullisionManager::getInstance()
{
	if (!_manager)
	{
		_manager = new BoxCullisionManager();
	}
	return _manager;
}

void BoxCullisionManager::registHeroBox(Vector<BoxBase*> vhb)
{
	m_heroBox.clear();
	m_heroBox = vhb;
}

void BoxCullisionManager::registEnemyBox(Vector<BoxBase*> veb)
{
	m_EnemyBox.clear();
	m_EnemyBox = veb;
}

void BoxCullisionManager::addHeroBox(BoxBase* h)
{
	m_heroBox.pushBack(h);
}

void BoxCullisionManager::addEnemyBox(BoxBase* e)
{
	m_EnemyBox.pushBack(e);
}

void BoxCullisionManager::eraseHeroBox(BoxBase* h)
{
	m_heroBox.eraseObject(h);
}

void BoxCullisionManager::eraseEnemyBox(BoxBase* e)
{
	m_EnemyBox.eraseObject(e);
}

void BoxCullisionManager::removeAllHeorBox()
{
	m_heroBox.clear();
}

void BoxCullisionManager::removeAllEnemyBox()
{
	m_EnemyBox.clear();
}

bool BoxCullisionManager::hitEnemy(BoxBase* h)
{
	bool ret = false;
	auto et = TargetManager::getInstance()->getEnemyTargets();
	for (auto e : et)
	{
		if (h->getBox().containsPoint(e->getPosition()))
		{
			e->getDeBuff(h->getDeBuff());
			ret = true;
		}
	}
	return ret;
}

bool BoxCullisionManager::hitHero(BoxBase* e)
{
	bool ret = false;
	auto ht = TargetManager::getInstance()->getHeroTargets();
	for (auto h : ht)
	{
		if (e->getBox().containsPoint(h->getPosition()))
		{
			h->getDeBuff(e->getDeBuff());//
			ret = true;
		}
	}
	return ret;
}

//TargetBase* BoxCullisionManager::getEnemyTarget(TargetBase* h)
//{
//	if (m_enemy.size() == 0)
//		return nullptr;
//	for (auto e : m_enemy)
//	{
//		if (h->convertToNodeSpace(e->convertToWorldSpace(Vec2::ZERO)).length() <= h->getAttCircleR())
//		{
//			return e;
//		}
//	}
//	return nullptr;
//}
//
//TargetBase* BoxCullisionManager::getHeroTarget(TargetBase* e)
//{
//	if (m_heros.size() == 0)
//		return nullptr;
//	for (auto h : m_heros)
//	{
//		if (e->convertToNodeSpace(h->convertToWorldSpace(Vec2::ZERO)).length() <= e->getAttCircleR())
//		{
//			return h;
//		}
//	}
//	return nullptr;
//}
//
//void BoxCullisionManager::registHero(Vector<HeroBase*> vh)
//{
//	m_heros.clear();
//	m_heros = vh;
//}
//
//void BoxCullisionManager::registEnemy(Vector<EnemyBase*> ve)
//{
//	m_enemy.clear();
//	m_enemy = ve;
//}
//
//void BoxCullisionManager::eraseHero(HeroBase* h)
//{
//	m_heros.eraseObject(h);
//}
//
//void BoxCullisionManager::eraseEnemy(EnemyBase* e)
//{
//	m_enemy.eraseObject(e);
//}
//
//void BoxCullisionManager::removeAllHeor()
//{
//	m_heros.clear();
//}
//
//void BoxCullisionManager::removeAllEnemy()
//{
//	m_enemy.clear();
//}
