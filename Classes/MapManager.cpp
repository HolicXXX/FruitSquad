#include "MapManager.h"
USING_NS_CC;

static MapManager* _mapmanager = nullptr;

MapManager::MapManager()
{

}

MapManager::~MapManager()
{

}

MapManager* MapManager::getInstance()
{
	if (!_mapmanager)
	{
		_mapmanager = new MapManager();
	}
	return _mapmanager;
}

void MapManager::registMap(cocos2d::TMXTiledMap* m)
{
	m_map = m;
	auto tile = m_map->getTileSize();
	auto size = m_map->getMapSize();
	m_mapSize = Size(tile.width * size.width, tile.height * size.height);
}