#ifndef _GOLDLABEL_H_
#define _GOLDLABEL_H_

#include "cocos2d.h"

class GoldLabel : public cocos2d::Node
{
public:
	static GoldLabel* create();
	virtual bool init();

	void getCoin(int c);
	
	void addGold(int g);
	void costGold(int g);

	void resetLabel();
private:
	int m_num;

	void initBG();
	cocos2d::Sprite* m_bg;
	cocos2d::Label* m_label;
};


#endif