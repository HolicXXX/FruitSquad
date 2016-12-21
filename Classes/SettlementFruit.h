#ifndef _SETTLEMENT_FRUIT_H_
#define _SETTLEMENT_FRUIT_H_

#include "cocos2d.h"

class SettlementFruit : public cocos2d::Node
{
public:
	static SettlementFruit* create(bool isget,int num);
	virtual bool init(bool isget,int num);

	void getFruit();

	bool isGet(){ return m_isGet; }
	int getNum(){ return m_num; }
private:
	bool m_isGet;
	int m_num;
	cocos2d::Sprite* m_fruit;
	cocos2d::Sprite* m_mask;
};

#endif