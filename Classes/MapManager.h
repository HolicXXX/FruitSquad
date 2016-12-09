#ifndef _MAPMANAGER_H_
#define _MAPMANAGER_H_

#include "cocos2d.h"

class MapManager
{
public:
	static MapManager* getInstance();

	void registMap(cocos2d::TMXTiledMap* m);
	cocos2d::TMXTiledMap* getMap(){ return m_map; }

	cocos2d::Size getMapSize(){ return m_mapSize; }
private:
	MapManager();
	~MapManager();

	cocos2d::TMXTiledMap* m_map;
	cocos2d::Size m_mapSize;
};


#endif