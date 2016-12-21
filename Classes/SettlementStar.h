#ifndef _SETTLEMENT_STAR_H_
#define _SETTLEMENT_STAR_H_

#include "cocos2d.h"

class SettlementStar : public cocos2d::Node
{
public:
	static SettlementStar* create();
	virtual bool init();

	void addStar();
private:
	int m_starNum;
	cocos2d::Sprite* m_bg;
	cocos2d::Vector<cocos2d::Sprite*> m_stars;
};


#endif