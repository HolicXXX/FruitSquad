#ifndef _LEVELUP_BUTTON_H_
#define _LEVELUP_BUTTON_H_

#include "HeadFile.h"
#include "Level.h"

class LevelUpButton : public Node
{
public:
	static LevelUpButton* create(Level l,int cost);
	virtual bool init(Level l, int cost);

	void resetButton(Level l, int cost);
	void bindLevelUpCallFunc(const std::function<void()>& func){ m_callback = func; }
private:
	Sprite* m_button;
	Label* m_label;
	Level m_level;
	int m_goldCost;
	bool m_isEnough;

	std::function<void()> m_callback;
	void observerCallBack(Ref*);
	void initTouchListener();
};



#endif