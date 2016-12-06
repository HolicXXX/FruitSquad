#ifndef _TEAMLIST_H_
#define _TEAMLIST_H_

#include "cocos2d.h"

class TeamList : public cocos2d::Node
{
public:
	static TeamList* create();
	virtual bool init();

	int getCount(){ return m_count; }
	bool isFull(){ return m_count == m_maxNum; }
	void addHero(int index);
	std::vector<int> getIndexList(){ return m_index; }
	void getTouched(cocos2d::Touch* t);
	cocos2d::Rect getBox(){ return m_listRect; }
private:
	int m_count;
	int m_maxNum;
	int m_mustChooseIndex;
	int m_mustChoosePos;
	cocos2d::Rect m_listRect;

	void resetIcon();
	std::vector<int> m_index;
	int isFind(int index);
	void reSort(bool must,int index);
	int detectTouch(cocos2d::Touch* t);

	void initBG();
	cocos2d::Vector<cocos2d::Node*> m_listBG;
};


#endif