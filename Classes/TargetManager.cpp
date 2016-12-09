#include "TargetManger.h"

static TargetManager* _targetmanager = nullptr;

TargetManager* TargetManager::getInstance()
{
	if (!_targetmanager)
	{
		_targetmanager = new TargetManager();
	}
	return _targetmanager;
}

TargetBase* TargetManager::getEnemyTarget(TargetBase* h)
{
	if (m_enemy.size() == 0)
		return nullptr;
	for (auto e : m_enemy)
	{
		if (h->convertToNodeSpace(e->convertToWorldSpace(Vec2::ZERO)).length() <= h->getAttCircleR())
		{
			return e;
		}
	}
	return nullptr;
}

TargetBase* TargetManager::getHeroTarget(TargetBase* e)
{
	if (m_heros.size() == 0)
		return nullptr;
	for (auto h : m_heros)
	{
		if (e->convertToNodeSpace(h->convertToWorldSpace(Vec2::ZERO)).length() <= e->getAttCircleR())
		{
			return h;
		}
	}
	return nullptr;
}

void TargetManager::registHero(Vector<HeroBase*> vh)
{
	m_heros.clear();
	m_heros = vh;
}

void TargetManager::registEnemy(Vector<EnemyBase*> ve)
{
	m_enemy.clear();
	m_enemy = ve;
}

void TargetManager::eraseHero(HeroBase* h)
{
	m_heros.eraseObject(h);
}

void TargetManager::eraseEnemy(EnemyBase* e)
{
	m_enemy.eraseObject(e);
}

void TargetManager::removeAllHeor()
{
	m_heros.clear();
}

void TargetManager::removeAllEnemy()
{
	m_enemy.clear();
}

Vector<EnemyBase*> TargetManager::getEnemyTargets()
{
	return m_enemy;
}

Vector<HeroBase*> TargetManager::getHeroTargets()
{
	return m_heros;
}

