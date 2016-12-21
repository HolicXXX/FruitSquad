#ifndef _TIPSLAYER_H_
#define _TIPSLAYER_H_

#include "HeadFile.h"

class TipsLayer
{
public:
	static TipsLayer* getInstance();

	cocos2d::Layer* getNEGemsLayer();
	cocos2d::Layer* getNEFruitLayer();
private:
	TipsLayer(){};
	~TipsLayer(){};
};


#endif